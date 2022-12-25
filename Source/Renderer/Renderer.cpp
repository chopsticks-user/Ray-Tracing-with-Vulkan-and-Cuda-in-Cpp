#include "Renderer.hpp"

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif /* TINYOBJLOADER_IMPLEMENTATION */
#include "tiny_obj_loader.h"

#include <iostream>
#include <unordered_map>

namespace Neko
{
    Renderer::Renderer(const EngineConfigs &crConfigs, ThreadPool &threadPool)
        : modelData{loadModel("Data/Resources/Models/VikingRoom.obj")},

          mpConfigs{&crConfigs},

          mpThreadPool{&threadPool},

          mInstance{*mpConfigs},

          mWindow{*mpConfigs, Window::resizeable},

          mSurface{mInstance, mWindow},

          mDevice{mInstance, mSurface},

          mCommandPool{mDevice},

          mCommandBuffers{mCommandPool.alloc(vku32(maxFramesInFlight))},

          mSwapchain{crConfigs, mSurface, mDevice},

          mRenderSync{mDevice, maxFramesInFlight},

          mDepthBuffer{crConfigs, mDevice, mSwapchain},

          mUniformBuffers{maxFramesInFlight},

          mVertexBuffer{mDevice, mCommandPool, modelData.vertices.data(),
                        sizeof(ShaderObject::Vertex) * modelData.vertices.size()},

          mIndexBuffer{mDevice, mCommandPool, modelData.indices.data(),
                       sizeof(ShaderObject::Index) * modelData.indices.size()},

          mTextureImage{"Data/Resources/Models/VikingRoom.png",
                        mDevice, mCommandPool},

          mSampler{crConfigs, mDevice},

          mDescriptorSetLayout{crConfigs, mDevice},

          mDescriptorPool{crConfigs, mDevice, vku32(maxFramesInFlight)},

          mDescriptorSets{mDescriptorPool.alloc({maxFramesInFlight,
                                                 *mDescriptorSetLayout})},

          mRenderPass{mDevice, mSwapchain, mDepthBuffer},

          mFramebuffers{mDevice, mSwapchain, mRenderPass, mDepthBuffer},

          mPipelineLayout{mDevice, {*mDescriptorSetLayout}},

          mGraphicsPipeline{
              crConfigs,
              mDevice,
              mSwapchain,
              mRenderPass,
              mPipelineLayout,
              {
                  {ShaderStage::fragment, "Data/Resources/Shaders/Basic.frag.spv"},
                  {ShaderStage::vertex, "Data/Resources/Shaders/Basic.vert.spv"},
              },
          }
    {

        /* Uniform buffers */
        for (auto &uniformBuffer : mUniformBuffers)
        {
            uniformBuffer = {mDevice, sizeof(ShaderObject::Uniform)};
        }

        /* Descriptor sets */
        updateDesciptorSets();
    }

    Renderer::~Renderer() = default;

    void Renderer::start()
    {
        while (!glfwWindowShouldClose(*mWindow))
        {
            glfwPollEvents();
            renderProcess();
        }
        vkDeviceWaitIdle(*mDevice);
    }

    void Renderer::updateDesciptorSets()
    {
        for (u64 i = 0; i < maxFramesInFlight; ++i)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = *mUniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(ShaderObject::Uniform);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = mTextureImage.view();
            imageInfo.sampler = *mSampler;

            std::array<VkWriteDescriptorSet, 2> writeSets{};

            writeSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSets[0].dstSet = mDescriptorSets[i];
            writeSets[0].dstBinding = 0;
            writeSets[0].dstArrayElement = 0;
            writeSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSets[0].descriptorCount = 1;
            writeSets[0].pBufferInfo = &bufferInfo;
            writeSets[0].pImageInfo = nullptr;
            writeSets[0].pTexelBufferView = nullptr;

            writeSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSets[1].dstSet = mDescriptorSets[i];
            writeSets[1].dstBinding = 1;
            writeSets[1].dstArrayElement = 0;
            writeSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeSets[1].descriptorCount = 1;
            writeSets[1].pBufferInfo = nullptr;
            writeSets[1].pImageInfo = &imageInfo;
            writeSets[1].pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(*mDevice, vku32(writeSets.size()), writeSets.data(),
                                   0, nullptr);
        }
    }

    ShaderObject::Uniform
    Renderer::getUniformBuffer([[maybe_unused]] float elapsedTime)
    {
        ShaderObject::Uniform ubo;
        ubo.model = glm::rotate(glm::mat4(1.0f), elapsedTime * glm::radians(90.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view =
            glm::lookAt(glm::vec3{2.0f, 2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
                        glm::vec3{0.0f, 0.0f, 1.0f});
        ubo.proj =
            glm::perspective(glm::radians(45.0f),
                             static_cast<float>(mSwapchain.extent().width) /
                                 static_cast<float>(mSwapchain.extent().height),
                             0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        return ubo;
    }

    void Renderer::updateFrame(u32 currentFrame)
    {
        static const auto startTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(
                         currentTime - startTime)
                         .count();
        auto updatedUniformBuffer = getUniformBuffer(time);
        mUniformBuffers[currentFrame].copy(&updatedUniformBuffer,
                                           sizeof(updatedUniformBuffer));
    }

    void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                       u32 imageIndex)
    {
        mCommandPool.beginBuffer(commandBuffer);

        /* Start a render pass */
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = *mRenderPass;
        renderPassBeginInfo.framebuffer = mFramebuffers[imageIndex];
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = mSwapchain.extent();
        renderPassBeginInfo.clearValueCount = vku32(clearValues.size());
        renderPassBeginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        /* Drawing commands */
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          *mGraphicsPipeline);

        /* Bind the buffers */
        VkBuffer vertexBuffers[] = {*mVertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        // /* Bind the descriptor sets */
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                *mPipelineLayout, 0, 1, mDescriptorSets.data(), 0,
                                nullptr);

        /* Bind if using index buffers */
        vkCmdBindIndexBuffer(commandBuffer, *mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        /* Draw the index buffers */
        static constexpr u32 instanceCount = 1;
        static constexpr u32 firstIndex = 0;
        static constexpr u32 vertexOffset = 0;
        static constexpr u32 firstInstance = 0;
        vkCmdDrawIndexed(commandBuffer, vku32(modelData.indices.size()),
                         instanceCount, firstIndex, vertexOffset, firstInstance);

        vkCmdEndRenderPass(commandBuffer);
        mCommandPool.endBuffer(commandBuffer);
    }

    void Renderer::renderProcess()
    {
        /* Wait for the previous frame to finish */
        auto currentFrame = mRenderSync.currentFrame();
        vkWaitForFences(*mDevice, 1, &mRenderSync.inFlightFence(currentFrame),
                        VK_TRUE, UINT64_MAX);

        /* Acquire an image frome the swapchain */
        uint32_t imageIndex;
        VkResult result =
            vkAcquireNextImageKHR(*mDevice, *mSwapchain, UINT64_MAX,
                                  mRenderSync.imageAvailableSemaphore(currentFrame),
                                  VK_NULL_HANDLE, &imageIndex);

        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapchain();
            return;
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to acquire swapchain image.");
        }

        /* Update after the acquired swapchain image is available */
        updateFrame(vku32(currentFrame));

        vkResetFences(*mDevice, 1, &mRenderSync.inFlightFence(currentFrame));

        /* Record a command buffer which draws the scene onto that image */
        vkResetCommandBuffer(mCommandBuffers[currentFrame], 0);
        recordCommandBuffer(mCommandBuffers[currentFrame], imageIndex);

        /* Submit the recorded command buffer */
        std::vector<VkSemaphore> waitSemaphores = {
            mRenderSync.imageAvailableSemaphore(currentFrame)};
        std::vector<VkPipelineStageFlags> waitStages = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        std::vector<VkSemaphore> signalSemaphores = {
            mRenderSync.renderFinishedSemaphore(currentFrame)};

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = vku32(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mCommandBuffers[currentFrame];
        submitInfo.signalSemaphoreCount = vku32(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        mCommandPool.submit(&submitInfo, mRenderSync.inFlightFence(currentFrame));

        /* Present the swapchain image */
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores.data();

        VkSwapchainKHR swapChains[] = {mSwapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(mDevice.queue().mQueue, &presentInfo);
        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR ||
            resized)
        {
            resized = false;
            recreateSwapchain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swapchain image.");
        }

        mRenderSync.next();
    }

    void Renderer::resizeCallback(GLFWwindow *window, [[maybe_unused]] i32 width,
                                  [[maybe_unused]] i32 height)
    {
        auto renderer =
            reinterpret_cast<Renderer *>(glfwGetWindowUserPointer(window));
        renderer->resized = true;
    }

    void Renderer::recreateSwapchain()
    {
        mWindow.whileMinimize();
        mDevice.waitIdle();

        mGraphicsPipeline.release();
        mFramebuffers.release();
        mRenderPass.release();
        mDepthBuffer.release();
        mSwapchain.release();

        mSwapchain = {*mpConfigs, mSurface, mDevice};
        mDepthBuffer = {*mpConfigs, mDevice, mSwapchain};
        mRenderPass = {mDevice, mSwapchain, mDepthBuffer};
        mFramebuffers = {mDevice, mSwapchain, mRenderPass, mDepthBuffer};
        mGraphicsPipeline = {
            *mpConfigs,
            mDevice,
            mSwapchain,
            mRenderPass,
            mPipelineLayout,
            {
                {ShaderStage::fragment, "Data/Resources/Shaders/Basic.frag.spv"},
                {ShaderStage::vertex, "Data/Resources/Shaders/Basic.vert.spv"},
            },
        };
    }

    ModelVertexData Renderer::loadModel(const std::string &modelPath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              modelPath.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        ModelVertexData loadedModel{};
        std::unordered_map<ShaderObject::Vertex, ShaderObject::Index> uniqueVertices{};

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                ShaderObject::Vertex vertex{};
                vertex.position = {attrib.vertices[3 * index.vertex_index + 0],
                                   attrib.vertices[3 * index.vertex_index + 1],
                                   attrib.vertices[3 * index.vertex_index + 2]};
                vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                                   1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] =
                        static_cast<uint32_t>(loadedModel.vertices.size());
                    loadedModel.vertices.push_back(vertex);
                }
                loadedModel.indices.push_back(uniqueVertices[vertex]);
            }
        }
        return loadedModel;
    }

} // namespace Neko