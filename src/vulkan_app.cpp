#include "vulkan_app.hpp"

void VulkanApp::recreateSwapchain() {
  /* Handle minimization */
  window.whileMinimized();

  /* Wait until all resources are not in used */
  device.waitIdle();

  /* Free current swapchain and its dependencies */
  framebuffers = {};
  graphicsPipeline = {};
  imageViews = {};
  swapchain = {};

  /* Recreate swapchain and its dependencies */
  swapchain = {surface.ref(), device.ref(), device.physical(),
               preferredPresentMode};
  imageViews = {device.ref(), swapchain.ref(), swapchain.format()};
  graphicsPipeline = {device.ref(),
                      swapchain.extent(),
                      swapchain.format(),
                      &descriptorSetLayout.ref(),
                      "/build/shaders/triangle_vert.spv",
                      "/build/shaders/triangle_frag.spv"};
  framebuffers = {device.ref(), imageViews.ref(), graphicsPipeline.renderPass(),
                  swapchain.extent()};
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
  VkCommandBufferBeginInfo cmdBufferBeginInfo{};
  cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmdBufferBeginInfo.flags = 0;
  cmdBufferBeginInfo.pInheritanceInfo = nullptr;
  vkh::beginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo);

  /* 2. Start a render pass */
  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

  VkRenderPassBeginInfo renderPassBeginInfo{};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = graphicsPipeline.renderPass();
  renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
  renderPassBeginInfo.renderArea.offset = {0, 0};
  renderPassBeginInfo.renderArea.extent = swapchain.extent();
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearColor;

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
  vkCmdBindIndexBuffer(cmdBuffer, indexBuffer.ref(), 0, VK_INDEX_TYPE_UINT16);

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
  vkCmdDrawIndexed(cmdBuffer,
                   static_cast<uint32_t>(shader::triangle_indices.size()),
                   instanceCount, firstIndex, vertexOffset, firstInstance);

  /* 4. End recording the command buffer */
  vkCmdEndRenderPass(cmdBuffer);
  vkh::endCommandBuffer(cmdBuffer);
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
  VkDeviceSize bufferSize = sizeof(shader::triangle_index_data[0]) *
                            shader::triangle_index_data.size();

  /* Create a host visible buffer as temporary buffer */
  vkw::Buffer stagingBuffer{device.ref(), device.physical(), bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  /* Copy the vertices data to the staging buffer */
  stagingBuffer.copyHostData(shader::triangle_index_data.data(), bufferSize);

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
  VkDeviceSize bufferSize =
      sizeof(shader::triangle_indices[0]) * shader::triangle_indices.size();

  /* Create a host visible buffer as temporary buffer */
  vkw::Buffer stagingBuffer = {device.ref(), device.physical(), bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  /* Copy the vertices data to the staging buffer */
  stagingBuffer.copyHostData(shader::triangle_indices.data(), bufferSize);

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
  VkDeviceSize bufferSize = sizeof(vkh::UniformBufferObject);
  std::vector<vkw::Buffer> buffers{maxFramesInFlight};
  for (auto &buffer : buffers) {
    buffer = {device.ref(), device.physical(), bufferSize,
              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
  }
  return buffers;
}

void VulkanApp::updateUniformBuffer(uint32_t currentImage) {
  static const auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  vkh::UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(360.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),
                              static_cast<float>(swapchain.extent().width) /
                                  swapchain.extent().height,
                              0.1f, 10.0f);
  ubo.proj[1][1] *= -1;

  void *data;
  vkMapMemory(device.ref(), uniformBuffers[currentImage].memory(), 0,
              sizeof(ubo), 0, &data);
  std::memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(device.ref(), uniformBuffers[currentImage].memory());
}

vkw::DescriptorSets VulkanApp::makeDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts{maxFramesInFlight,
                                             descriptorSetLayout.ref()};
  auto sets = descriptorPool.allocateSets(layouts);
  std::vector<VkWriteDescriptorSet> descriptorWrites{};
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i].ref();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(vkh::UniformBufferObject);

    VkWriteDescriptorSet writeSet{};
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.dstSet = sets[i];
    writeSet.dstBinding = 0;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeSet.descriptorCount = 1;
    writeSet.pBufferInfo = &bufferInfo;
    writeSet.pImageInfo = nullptr;
    writeSet.pTexelBufferView = nullptr;
    descriptorWrites.emplace_back(writeSet);
  }
  descriptorPool.updateSets(descriptorWrites);
  return sets;
}

vkw::Image VulkanApp::makeTextureImage() {
  int imageWidth, imageHeight, imageChannels;
  stbi_uc *pixels = stbi_load(imagePath.c_str(), &imageWidth, &imageHeight,
                              &imageChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = imageWidth * imageHeight * 4;
  if (!pixels) {
    throw std::runtime_error("Failed to load image.");
  }
  vkw::Buffer stagingBuffer = {device.ref(), device.physical(), imageSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
  stagingBuffer.copyHostData(pixels, imageSize);
  stbi_image_free(pixels);
  return {device.ref(),
          device.physical(),
          static_cast<uint32_t>(imageWidth),
          static_cast<uint32_t>(imageHeight),
          VK_FORMAT_R8G8B8A8_SRGB,
          VK_IMAGE_TILING_OPTIMAL,
          VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT};
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
  updateUniformBuffer(static_cast<uint32_t>(sync.currentFrame));

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
}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window.ref())) {
    glfwPollEvents();
    render();
  }
  device.waitIdle();
}

void VulkanApp::writeInfo(std::string filePath) {
  std::fstream fs;
  fs.open(filePath);
  if (!fs.is_open()) {
    throw std::runtime_error("Failed to open " + filePath + '\n');
  }

  fs << "1. Instance extensions:\n";
  fs << "\t1.1 Available:\n";
  auto availableInstanceExtensions = vkh::getAvailableInstanceExtensionList();
  for (const auto &extension : availableInstanceExtensions) {
    fs << "\t\t" << extension.extensionName << '\n';
  }
  fs << "\t1.2 Required:\n";
  for (const auto &extensionName : instanceExtensions) {
    fs << "\t\t" << extensionName << '\n';
  }
  fs << '\n';

  fs << "2. Instance layers:\n";
  fs << "\t2.1 Available:\n";
  auto availableInstanceLayers = vkh::getAvailableInstanceLayerList();
  for (const auto &layer : availableInstanceLayers) {
    fs << "\t\t" << layer.layerName << '\n';
  }
  fs << "\t2.2 Required:\n";
  for (const auto &layer : instanceLayers) {
    fs << "\t\t" << layer << '\n';
  }
  fs << '\n';

  fs << "3. Physical devices:\n";
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance.ref());
  auto selectedDeviceProperties =
      vkh::getPhysicalDevicePropertyList(device.physical());
  size_t index = 1;
  size_t selectedDeviceIndex = 0;
  for (const auto &physDev : physicalDeviceList) {
    std::string selectStr = "";

    VkPhysicalDeviceVulkan11Properties vk11Props{};
    vk11Props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
    vk11Props.pNext = nullptr;

    VkPhysicalDeviceVulkan12Properties vk12Props{};
    vk12Props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
    vk12Props.pNext = &vk11Props;

    VkPhysicalDeviceVulkan13Properties vk13Props{};
    vk13Props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
    vk13Props.pNext = &vk12Props;

    auto deviceProperties =
        vkh::getPhysicalDevicePropertyList(physDev, &vk13Props);

    if (deviceProperties.properties.deviceID ==
        selectedDeviceProperties.deviceID) {
      selectedDeviceIndex = index;
    }
    if (selectedDeviceIndex == index) {
      selectStr = " (selected)";
    }
    fs << "\t3." << index << " Device " << index << selectStr << ":\n";
    fs << "\t\t3." << index << ".1 Device properties:\n";
    fs << "\t\t\tDevice ID: " << deviceProperties.properties.deviceID << '\n';
    fs << "\t\t\tDevice name: " << deviceProperties.properties.deviceName
       << '\n';
    fs << "\t\t\tDevice type: ";
    switch (deviceProperties.properties.deviceType) {
    case 1:
      fs << "Integrated GPU\n";
      break;
    case 2:
      fs << "Discrete GPU\n";
      break;
    case 3:
      fs << "Virtual GPU\n";
      break;
    case 4:
      fs << "CPU\n";
      break;
    default:
      fs << "Other\n";
      break;
    }
    fs << "\t\t\tDriver ID: " << vk12Props.driverID << '\n';
    fs << "\t\t\tDriver name: " << vk12Props.driverName << '\n';
    fs << "\t\t\tDriver version: " << vk12Props.driverInfo << '\n';
    // fs << "\t\t\tDriver version: " << vk13Props. << '\n';
    fs << "\t\t\tMaximum memory allocation size: "
       << vk11Props.maxMemoryAllocationSize << '\n';
    fs << "\t\t3." << index << ".2 Available device extensions:\n";
    auto availableDeviceExtensions =
        vkh::getAvailableDeviceExtensionList(physDev);
    for (const auto &extension : availableDeviceExtensions) {
      fs << "\t\t\t" << extension.extensionName << '\n';
    }
    if (selectedDeviceIndex == index) {
      fs << "\t\t3." << index << ".3 Required device extensions:\n";
      for (const auto &extension : deviceExtensions) {
        fs << "\t\t\t" << extension << '\n';
      }
    }
    fs << '\n';
    ++index;
  }
  fs << '\n';
}
