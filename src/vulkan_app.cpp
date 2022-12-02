#include "vulkan_app.hpp"

namespace rtvc {

void VulkanApp::recreateSwapchain() {
  /* Handle minimization */
  window.whileMinimized();

  /* Wait until all resources are not in used */
  device.waitIdle();

  /* Free current swapchain and its dependencies */
  framebuffers = {};
  colorImage = {};
  colorImageView = {};
  depthImage = {};
  depthImageView = {};
  graphicsPipeline = {};
  imageViews = {};
  swapchain = {};

  /* Recreate swapchain and its dependencies */
  swapchain = {surface, device, preferredPresentMode};
  imageViews = {device, swapchain};
  depthFormat = findDepthFormat();
  graphicsPipeline = {{
      device,
      swapchain,
      depthFormat,
      msaaSamples,
      descriptorSetLayout,
      "/build/shaders/triangle_vert.spv",
      "/build/shaders/triangle_frag.spv",
  }};
  colorImage = {device,
                {
                    swapchain.extent().width,
                    swapchain.extent().height,
                    1,
                    msaaSamples,
                    swapchain.format(),
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                }};
  colorImageView = {
      device, colorImage, swapchain.format(), VK_IMAGE_ASPECT_COLOR_BIT, 1,
  };
  depthImage = makeDepthImage(depthFormat);
  depthImageView = makeDepthView(depthImage, depthFormat);
  framebuffers = {
      device,         imageViews,       depthImageView,
      colorImageView, graphicsPipeline, swapchain,
  };
}

void VulkanApp::framebufferResizeCallback(GLFWwindow *pWindow,
                                          [[maybe_unused]] int width,
                                          [[maybe_unused]] int height) {
  auto app = reinterpret_cast<VulkanApp *>(glfwGetWindowUserPointer(pWindow));
  app->framebufferResized = true;
}

void VulkanApp::recordCommandBuffer(VkCommandBuffer cmdBuffer,
                                    uint32_t imageIndex) {
  /* 1. Begin recording a command buffer */
  commandPool.beginBuffer(cmdBuffer, 0);

  /* 2. Start a render pass */
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  VkRenderPassBeginInfo renderPassBeginInfo{};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = graphicsPipeline.renderPass();
  renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
  renderPassBeginInfo.renderArea.offset = {0, 0};
  renderPassBeginInfo.renderArea.extent = swapchain.extent();
  renderPassBeginInfo.clearValueCount =
      static_cast<uint32_t>(clearValues.size());
  renderPassBeginInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  /* 3. Drawing commands */
  vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline.ref());

  /* Bind the buffers */
  VkBuffer vertexBuffers[] = {vertexBuffer.ref()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

  /* Bind the descriptor sets */
  vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphicsPipeline.layout(), 0, 1,
                          descriptorSets.ref().data(), 0, nullptr);

  /* Bind if using index buffers */
  vkCmdBindIndexBuffer(cmdBuffer, indexBuffer.ref(), 0, VK_INDEX_TYPE_UINT32);

  /* Draw the vertex buffers */
  // static const uint32_t instanceCount = 1;
  // static const uint32_t firstVertex = 0;
  // static const uint32_t firstInstance = 0;
  // vkCmdDraw(cmdBuffer,
  //           static_cast<uint32_t>(shader::triangle_vertex_data.size()),
  //           instanceCount, firstVertex, firstInstance);

  /* Draw the index buffers */
  static const uint32_t instanceCount = 1;
  static const uint32_t firstIndex = 0;
  static const uint32_t vertexOffset = 0;
  static const uint32_t firstInstance = 0;
  vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(model.indices.size()),
                   instanceCount, firstIndex, vertexOffset, firstInstance);

  /* 4. End recording the command buffer */
  vkCmdEndRenderPass(cmdBuffer);
  commandPool.endBuffer(cmdBuffer);
}

VulkanApp::ModelWrapper VulkanApp::loadModel() {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        modelObjectPath.c_str())) {
    throw std::runtime_error(warn + err);
  }

  ModelWrapper loadedModel{};
  std::unordered_map<vkw::Vertex, uint32_t> uniqueVertices{};

  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      vkw::Vertex vertex{};
      vertex.position = {attrib.vertices[3 * index.vertex_index + 0],
                         attrib.vertices[3 * index.vertex_index + 1],
                         attrib.vertices[3 * index.vertex_index + 2]};
      vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                         attrib.texcoords[2 * index.texcoord_index + 1]};
      vertex.color = {1.0f, 1.0f, 1.0f};

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] =
            static_cast<uint32_t>(loadedModel.vertices.size());
        loadedModel.vertices.push_back(vertex);
      }
      loadedModel.indices.push_back(uniqueVertices[vertex]);
    }
  }
  return loadedModel;
}

uint32_t VulkanApp::findMemoryType(uint32_t typeFilter,
                                   VkMemoryPropertyFlags propFlags) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(device.physical(), &memoryProperties);
  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
    if (typeFilter & (1 << i) &&
        (memoryProperties.memoryTypes[i].propertyFlags & propFlags) ==
            propFlags) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type.");
}

vkw::Buffer VulkanApp::makeVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(model.vertices[0]) * model.vertices.size();

  /* Create a host visible buffer as temporary buffer */
  vkw::Buffer stagingBuffer{device.ref(), device.physical(), bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  /* Copy the vertices data to the staging buffer */
  stagingBuffer.copyHostData(model.vertices.data(), bufferSize);

  /* Create a device local buffer as actual vertex buffer */
  vkw::Buffer buffer = {device.ref(), device.physical(), bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT};

  /* Copy the data from the staging buffer to the device buffer */
  buffer.copyDeviceData(commandPool, device.queue(), stagingBuffer, bufferSize);
  return buffer;
}

vkw::Buffer VulkanApp::makeIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(model.indices[0]) * model.indices.size();

  /* Create a host visible buffer as temporary buffer */
  vkw::Buffer stagingBuffer = {device.ref(), device.physical(), bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  /* Copy the vertices data to the staging buffer */
  stagingBuffer.copyHostData(model.indices.data(), bufferSize);

  /* Create a device local buffer as actual vertex buffer */
  vkw::Buffer buffer = {device.ref(), device.physical(), bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT};

  /* Copy the data from the staging buffer to the device buffer */
  buffer.copyDeviceData(commandPool, device.queue(), stagingBuffer, bufferSize);
  return buffer;
}

std::vector<vkw::Buffer> VulkanApp::makeUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(vkw::UniformBufferObject);
  std::vector<vkw::Buffer> buffers{maxFramesInFlight};
  for (auto &buffer : buffers) {
    buffer = {device.ref(), device.physical(), bufferSize,
              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
  }
  return buffers;
}

void VulkanApp::scrollEventCallback(GLFWwindow *pWindow,
                                    [[maybe_unused]] double xOffset,
                                    double yOffset) {
  auto app = reinterpret_cast<VulkanApp *>(glfwGetWindowUserPointer(pWindow));
  app->currentScale += app->incScale * yOffset;
  if (app->currentScale > app->maxScale) {
    app->currentScale = app->maxScale;
  } else if (app->currentScale < app->minScale) {
    app->currentScale = app->minScale;
  }
}

void VulkanApp::keyboardEventCallback(GLFWwindow *pWindow,
                                      [[maybe_unused]] int key,
                                      [[maybe_unused]] int scancode,
                                      [[maybe_unused]] int action,
                                      [[maybe_unused]] int mods) {
  auto app = reinterpret_cast<VulkanApp *>(glfwGetWindowUserPointer(pWindow));
  if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
    app->paused = !app->paused;
  }
}

vkw::UniformBufferObject VulkanApp::updateUBO(float elapsedTime) {
  vkw::UniformBufferObject ubo{};
  if (!paused) {
    currentAngle = elapsedTime * angleVelocity;
  }
  ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(currentAngle),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  float cameraPosition = 2.0f / currentScale;
  ubo.view = glm::lookAt(
      glm::vec3{
          cameraPosition,
          cameraPosition,
          cameraPosition,
      },
      glm::vec3{
          0.0f,
          0.0f,
          0.0f,
      },
      glm::vec3{
          0.0f,
          0.0f,
          1.0f,
      });
  ubo.proj = glm::perspective(glm::radians(45.0f),
                              static_cast<float>(swapchain.extent().width) /
                                  swapchain.extent().height,
                              0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  return ubo;
}

void VulkanApp::updateFrame(uint32_t currentImage) {
  static const auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();
  auto ubo = updateUBO(time);
  uniformBuffers[currentImage].copyHostData(&ubo, sizeof(ubo));
}

vkw::DescriptorSets VulkanApp::makeDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts{maxFramesInFlight,
                                             descriptorSetLayout.ref()};
  auto sets = descriptorPool.allocateSets(layouts);
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i].ref();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(vkw::UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureImageView.ref();
    imageInfo.sampler = textureSampler.ref();

    std::array<VkWriteDescriptorSet, 2> writeSets{};

    writeSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSets[0].dstSet = sets[i];
    writeSets[0].dstBinding = 0;
    writeSets[0].dstArrayElement = 0;
    writeSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeSets[0].descriptorCount = 1;
    writeSets[0].pBufferInfo = &bufferInfo;
    writeSets[0].pImageInfo = nullptr;
    writeSets[0].pTexelBufferView = nullptr;

    writeSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSets[1].dstSet = sets[i];
    writeSets[1].dstBinding = 1;
    writeSets[1].dstArrayElement = 0;
    writeSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeSets[1].descriptorCount = 1;
    writeSets[1].pBufferInfo = nullptr;
    writeSets[1].pImageInfo = &imageInfo;
    writeSets[1].pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(device.ref(),
                           static_cast<uint32_t>(writeSets.size()),
                           writeSets.data(), 0, nullptr);
  }
  return sets;
}

VkSampleCountFlagBits VulkanApp::getMaxSampleCount() {
  if (!enableMSAA) {
    return VK_SAMPLE_COUNT_1_BIT;
  }

  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(device.physical(), &properties);
  VkSampleCountFlags counts = properties.limits.framebufferColorSampleCounts &
                              properties.limits.framebufferDepthSampleCounts;
  if (counts & VK_SAMPLE_COUNT_64_BIT) {
    return VK_SAMPLE_COUNT_64_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_32_BIT) {
    return VK_SAMPLE_COUNT_32_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_16_BIT) {
    return VK_SAMPLE_COUNT_16_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_8_BIT) {
    return VK_SAMPLE_COUNT_8_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_4_BIT) {
    return VK_SAMPLE_COUNT_4_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_2_BIT) {
    return VK_SAMPLE_COUNT_2_BIT;
  }
  return VK_SAMPLE_COUNT_1_BIT;
}

VkFormat VulkanApp::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                        VkImageTiling tiling,
                                        VkFormatFeatureFlags features) {
  for (auto format : candidates) {
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device.physical(), format, &properties);
    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (properties.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (properties.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw std::runtime_error("Failed to find supported format.");
}

VkFormat VulkanApp::findDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool VulkanApp::hasStencilComponent(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}

Image VulkanApp::makeDepthImage(VkFormat format) {
  Image image = {device,
                 {swapchain.extent().width, swapchain.extent().height, 1,
                  msaaSamples, format, VK_IMAGE_TILING_OPTIMAL,
                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}};
  transitionImageLayout(image.ref(), format, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  return image;
}

ImageView VulkanApp::makeDepthView(const Image &image, VkFormat format) {
  ImageView imageView = {device, image, format, VK_IMAGE_ASPECT_DEPTH_BIT};
  return imageView;
}

void VulkanApp::transitionImageLayout(VkImage image,
                                      [[maybe_unused]] VkFormat format,
                                      VkImageLayout oldLayout,
                                      VkImageLayout newLayout,
                                      uint32_t mipLevels) {
  auto commandBuffer = commandPool.allocateBuffer();
  commandPool.beginBuffer(commandBuffer);

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (hasStencilComponent(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = mipLevels;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = 0;
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = 0;
  VkPipelineStageFlags srcStage;
  VkPipelineStageFlags dstStage;
  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  } else {
    throw std::invalid_argument("Unsupported layout transition.");
  }
  vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0,
                       nullptr, 1, &barrier);

  commandPool.endBuffer(commandBuffer);
  commandPool.submitBuffer(device.queue(), commandBuffer);
  commandPool.freeBuffer(commandBuffer);
}

void VulkanApp::copyBufferToImage(VkBuffer buffer, VkImage image,
                                  uint32_t width, uint32_t height) {
  auto commandBuffer = commandPool.allocateBuffer();
  commandPool.beginBuffer(commandBuffer);

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};
  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  commandPool.endBuffer(commandBuffer);
  commandPool.submitBuffer(device.queue(), commandBuffer);
  commandPool.freeBuffer(commandBuffer);
}

void VulkanApp::generateMipmaps(VkImage image, VkFormat imageFormat,
                                int32_t width, int32_t height,
                                uint32_t mipLevels) {
  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(device.physical(), imageFormat,
                                      &formatProperties);
  if (!(formatProperties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    throw std::runtime_error("Image format does not support linear blitting.");
  }

  auto commandBuffer = commandPool.allocateBuffer();
  commandPool.beginBuffer(commandBuffer);

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.image = image;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.subresourceRange.levelCount = 1;

  int32_t mipWidth = width, mipHeight = height;
  for (uint32_t i = 1; i < mipLevels; ++i) {
    barrier.subresourceRange.baseMipLevel = i - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                         nullptr, 1, &barrier);
    VkImageBlit blit{};
    blit.srcOffsets[0] = {0, 0, 0};
    blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.mipLevel = i - 1;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount = 1;
    blit.dstOffsets[0] = {0, 0, 0};
    blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1,
                          mipHeight > 1 ? mipHeight / 2 : 1, 1};
    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.mipLevel = i;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount = 1;

    vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
                   VK_FILTER_LINEAR);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                         0, nullptr, 1, &barrier);

    if (mipWidth > 1) {
      mipWidth /= 2;
    }
    if (mipHeight > 1) {
      mipHeight /= 2;
    }
  }

  barrier.subresourceRange.baseMipLevel = mipLevels - 1;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                       VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
                       nullptr, 1, &barrier);

  commandPool.endBuffer(commandBuffer);
  commandPool.submitBuffer(device.queue(), commandBuffer);
  commandPool.freeBuffer(commandBuffer);
}

Image VulkanApp::makeTextureImage() {
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels = stbi_load(modelTexturePath.c_str(), &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;
  if (!pixels) {
    throw std::runtime_error("Failed to load image.");
  }
  vkw::Buffer stagingBuffer = {device.ref(), device.physical(), imageSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
  stagingBuffer.copyHostData(pixels, imageSize);
  stbi_image_free(pixels);
  _mipLevels = 1 + static_cast<uint32_t>(
                       std::floor(std::log2(std::max(texWidth, texHeight))));
  Image texImage = {
      device,
      {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
       _mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB,
       VK_IMAGE_TILING_OPTIMAL,
       VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
           VK_IMAGE_USAGE_SAMPLED_BIT,
       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}};
  transitionImageLayout(texImage.ref(), VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, _mipLevels);
  copyBufferToImage(stagingBuffer.ref(), texImage.ref(),
                    static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));
  generateMipmaps(texImage.ref(), VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight,
                  _mipLevels);
  return texImage;
}

void VulkanApp::render() {
  /* 1. Wait for the previous frame to finish */
  vkWaitForFences(device.ref(), 1, &sync.inFlightFence[sync.currentFrame].ref(),
                  VK_TRUE, UINT64_MAX);

  /* 2. Acquire an image frome the swapchain */
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      device.ref(), swapchain.ref(), UINT64_MAX,
      sync.imageAvailableSemaphore[sync.currentFrame].ref(), VK_NULL_HANDLE,
      &imageIndex);
  if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapchain();
    return;
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to acquire swapchain image.");
  }
  vkResetFences(device.ref(), 1, &sync.inFlightFence[sync.currentFrame].ref());

  /* 3. Record a command buffer which draws the scene onto that image */
  vkResetCommandBuffer(commandBuffers[sync.currentFrame], 0);
  recordCommandBuffer(commandBuffers[sync.currentFrame], imageIndex);

  /* 4. Submit the recorded command buffer */
  static const uint32_t submitCount = 1;
  std::vector<VkSemaphore> waitSemaphores = {
      sync.imageAvailableSemaphore[sync.currentFrame].ref()};
  std::vector<VkPipelineStageFlags> waitStages = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  std::vector<VkSemaphore> signalSemaphores = {
      sync.renderFinishedSemaphore[sync.currentFrame].ref()};

  /* Update after the acquired swapchain image is available */
  updateFrame(static_cast<uint32_t>(sync.currentFrame));

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
  submitInfo.pWaitSemaphores = waitSemaphores.data();
  submitInfo.pWaitDstStageMask = waitStages.data();
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[sync.currentFrame];
  submitInfo.signalSemaphoreCount =
      static_cast<uint32_t>(signalSemaphores.size());
  submitInfo.pSignalSemaphores = signalSemaphores.data();

  if (vkQueueSubmit(device.queue(), submitCount, &submitInfo,
                    sync.inFlightFence[sync.currentFrame].ref()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer.");
  }

  /* 5. Present the swapchain image */
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount =
      static_cast<uint32_t>(signalSemaphores.size());
  presentInfo.pWaitSemaphores = signalSemaphores.data();
  presentInfo.swapchainCount = swapchainCount;
  presentInfo.pSwapchains = &swapchain.ref();
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(device.queue(), &presentInfo);
  if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR ||
      framebufferResized) {
    framebufferResized = false;
    recreateSwapchain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swapchain image.");
  }

  /* 6. Set up the next frame */
  sync.currentFrame = (sync.currentFrame + 1) % maxFramesInFlight;
}

VulkanApp::VulkanApp() {
  glfwSetWindowUserPointer(window.ref(), this);
  glfwSetFramebufferSizeCallback(window.ref(), framebufferResizeCallback);
  glfwSetScrollCallback(window.ref(), scrollEventCallback);
  glfwSetKeyCallback(window.ref(), keyboardEventCallback);
}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window.ref())) {
    glfwPollEvents();
    render();
  }
  device.waitIdle();
}

} /* namespace rtvc */
