#include "renderer.hpp"

#include "threads.hpp"

namespace neko {

Renderer::Renderer(const Configs &crConfigs, ThreadPool &threadPool)
    : mpConfigs{&crConfigs}, mpThreadPool{&threadPool}, mInstance{*mpConfigs},
      mWindow{*mpConfigs}, mSurface{mInstance, mWindow}, mDevice{mInstance,
                                                                 mSurface},
      mCommandPool{mDevice}, mCommandBuffers{mCommandPool.alloc(
                                 vku32(maxFrameInFlights))},
      mSwapchain{crConfigs, mSurface, mDevice},
      mRenderSync{mDevice, maxFrameInFlights}, mDepthBuffer{crConfigs, mDevice,
                                                            mSwapchain},
      mUniformBuffers{maxFrameInFlights}, mSampler{crConfigs, mDevice},
      mDescriptorSetLayout{crConfigs, mDevice},
      mDescriptorPool{crConfigs, mDevice, vku32(maxFrameInFlights)},
      mRenderPass{mDevice, mSwapchain, mDepthBuffer},
      mFramebuffers{mDevice, mSwapchain, mRenderPass, mDepthBuffer} {

  /* Uniform buffers */
  for (auto &uniformBuffer : mUniformBuffers) {
    uniformBuffer = {mDevice, sizeof(shader_object::Uniform)};
  }

  /* Texture images */

  /* Descriptor sets */

  /* Graphics pipelines */
  mGraphicsPipeline = {
      crConfigs,
      mDevice,
      mSwapchain,
      mRenderPass,
      mDescriptorSetLayout,
      {{ShaderStage::vertex, "data/resources/shaders/basic.vert.spv"},
       {ShaderStage::fragment, "data/resources/shaders/basic.frag.spv"}},
  };
}

Renderer::~Renderer() = default;

void Renderer::start() {
  while (!glfwWindowShouldClose(*mWindow)) {
    glfwPollEvents();
    renderProcess();
  }
  vkDeviceWaitIdle(*mDevice);
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                   u32 imageIndex) {
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

  // /* Bind the buffers */
  // VkBuffer vertexBuffers[] = {vertexBuffer.ref()};
  // VkDeviceSize offsets[] = {0};
  // vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

  // /* Bind the descriptor sets */
  // vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  //                         graphicsPipeline.layout(), 0, 1,
  //                         descriptorSets.ref().data(), 0, nullptr);

  // /* Bind if using index buffers */
  // vkCmdBindIndexBuffer(cmdBuffer, indexBuffer.ref(), 0,
  // VK_INDEX_TYPE_UINT32);

  // /* Draw the index buffers */
  // static const uint32_t instanceCount = 1;
  // static const uint32_t firstIndex = 0;
  // static const uint32_t vertexOffset = 0;
  // static const uint32_t firstInstance = 0;
  // vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(model.indices.size()),
  //                  instanceCount, firstIndex, vertexOffset, firstInstance);

  vkCmdEndRenderPass(commandBuffer);
  mCommandPool.endBuffer(commandBuffer);
}

void Renderer::renderProcess() {
  /* 1. Wait for the previous frame to finish */
  auto currentFrame = mRenderSync.currentFrame();
  vkWaitForFences(*mDevice, 1, &mRenderSync.inFlightFence(currentFrame),
                  VK_TRUE, UINT64_MAX);
  vkResetFences(*mDevice, 1, &mRenderSync.inFlightFence(currentFrame));

  /* 2. Acquire an image frome the swapchain */
  uint32_t imageIndex;
  [[maybe_unused]] VkResult result =
      vkAcquireNextImageKHR(*mDevice, *mSwapchain, UINT64_MAX,
                            mRenderSync.imageAvailableSemaphore(currentFrame),
                            VK_NULL_HANDLE, &imageIndex);
  // if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
  //   recreateSwapchain();
  //   return;
  // } else if (result != VK_SUCCESS) {
  //   throw std::runtime_error("Failed to acquire swapchain image.");
  // }

  /* 3. Record a command buffer which draws the scene onto that image */
  vkResetCommandBuffer(mCommandBuffers[currentFrame], 0);
  recordCommandBuffer(mCommandBuffers[currentFrame], imageIndex);

  /* 4. Submit the recorded command buffer */
  static const uint32_t submitCount = 1;
  std::vector<VkSemaphore> waitSemaphores = {
      mRenderSync.imageAvailableSemaphore(currentFrame)};
  std::vector<VkPipelineStageFlags> waitStages = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  std::vector<VkSemaphore> signalSemaphores = {
      mRenderSync.renderFinishedSemaphore(currentFrame)};

  /* Update after the acquired swapchain image is available */
  // updateFrame(static_cast<uint32_t>(mRenderSync.currentFrame));

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
  submitInfo.pWaitSemaphores = waitSemaphores.data();
  submitInfo.pWaitDstStageMask = waitStages.data();
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &mCommandBuffers[currentFrame];
  submitInfo.signalSemaphoreCount =
      static_cast<uint32_t>(signalSemaphores.size());
  submitInfo.pSignalSemaphores = signalSemaphores.data();

  if (vkQueueSubmit(mDevice.queue().mQueue, submitCount, &submitInfo,
                    mRenderSync.inFlightFence(currentFrame)) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer.");
  }

  /* Present the swapchain image */
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores.data();

  VkSwapchainKHR swapChains[] = {*mSwapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(mDevice.queue().mQueue, &presentInfo);

  mRenderSync.next();
}

} /* namespace neko */