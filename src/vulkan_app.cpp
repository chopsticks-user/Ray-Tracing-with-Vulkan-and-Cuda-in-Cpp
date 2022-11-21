#include "vulkan_app.hpp"

void VulkanApp::createWindow() {
  glfwInit();
  window = vkh::createWindow(800, 600);
}

void VulkanApp::createInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  auto requiredExtensions = vkh::getRequiredInstanceExtensionNameList();
  for (auto &&extension : requiredExtensions) {
    instanceExtensions.emplace_back(extension);
  }

  auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};
  vkh::populateDebugMessengerCreateInfo(debugCreateInfo);

  VkInstanceCreateInfo instanceInfo;
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());
  instanceInfo.ppEnabledLayerNames = instanceLayers.data();
  instanceInfo.enabledExtensionCount =
      static_cast<uint32_t>(instanceExtensions.size());
  instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

  instance = vkh::createInstance(&instanceInfo);
}

void VulkanApp::createDebugMessenger() {
  if (vkh::checkValidationLayerSupport() == false) {
    throw std::runtime_error("Validation layers are not supported.");
  }
  debugMessenger = vkh::createDebugMessenger(instance);
}

void VulkanApp::createSurface() {
  surface = vkh::createSurface(instance, window);
}

bool VulkanApp::checkDeviceProperties(VkPhysicalDevice physDev) {
  auto physDevPropList = vkh::getPhysicalDevicePropertyList(physDev);
  return physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
VulkanApp::selectQueueFamily(VkPhysicalDevice physDev) {
  auto graphicsFamilies = vkh::getGraphicsQueueFamilyPropertyList(physDev);
  auto computeFamilies = vkh::getComputeQueueFamilyPropertyList(physDev);
  auto presentFamilies =
      vkh::getPresentQueueFamilyPropertyList(physDev, surface);

  /* Find a queue that is capable of handling graphics, computing, and
  supporting presentation to the surface */
  for (size_t i = 0; i < graphicsFamilies.size(); ++i) {
    if ((graphicsFamilies[i].has_value() && computeFamilies[i].has_value()) &&
        presentFamilies[i].has_value()) {
      return {{static_cast<uint32_t>(i), graphicsFamilies[i].value()}};
    }
  }
  return {};
}

void VulkanApp::createDevice() {
  /* Select a suitable physical device and one of its queue families */
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
  VkPhysicalDevice selectedPhysDev = VK_NULL_HANDLE;
  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
      selectedQueueFamily;
  for (const auto &physDev : physicalDeviceList) {
    /* Only selected a discrete GPU that has a queue family supporting
    graphics, computing, and presentation commands */
    /* When the application creates a swapchain, the selected physical device
    must support VK_KHR_swapchain */
    /* Also, the selected physical device must be compatible with the swapchain
     that will be created. */
    if (checkDeviceProperties(physDev) &&
        checkDeviceExtensionSupport(physDev) &&
        checkDeviceSwapchainSupport(physDev)) {
      if (auto returnedQueueFamily = selectQueueFamily(physDev);
          returnedQueueFamily.has_value()) {
        selectedQueueFamily = returnedQueueFamily;
        selectedPhysDev = physDev;
      }
    }
  }
  if (selectedPhysDev == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to select a physical device.");
  }

  /* Set up the selected queue family's creation info */
  uint32_t selectedIndex = selectedQueueFamily.value().first;
  const float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueInfo{};
  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueInfo.pNext = nullptr;
  queueInfo.queueFamilyIndex = selectedIndex;
  queueInfo.queueCount = 1;
  queueInfo.pQueuePriorities = &queuePriority;

  /* Create the logical device */
  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pNext = nullptr;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pQueueCreateInfos = &queueInfo;
  deviceCreateInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  deviceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(instanceLayers.size());
  deviceCreateInfo.ppEnabledLayerNames = instanceLayers.data();

  device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);

  /* Get a queue handle */
  vkGetDeviceQueue(device, selectedIndex, 0, &queue);

  /* Store the selected device for later uses */
  physicalDevice = selectedPhysDev;

  /* Store the index of the selected queue family */
  queueFamilyIndex = selectedIndex;
}

bool VulkanApp::checkDeviceExtensionSupport(VkPhysicalDevice physDev) {
  auto availableExtensionsList = vkh::getAvailableDeviceExtensionList(physDev);
  std::map<std::string, uint32_t> helper;
  for (const auto &availableExtension : availableExtensionsList) {
    helper[availableExtension.extensionName]++;
    // std::cout << availableExtension.extensionName << '\n';
  }
  for (const auto &deviceExtension : deviceExtensions) {
    helper[deviceExtension]++;
    if (helper[deviceExtension] < 2) {
      return false;
    }
  }
  return true;
}

bool VulkanApp::checkDeviceSwapchainSupport(
    [[maybe_unused]] VkPhysicalDevice physDev) {
  return true;
}

VkSwapchainCreateInfoKHR VulkanApp::populateSwapchainCreateInfo() {
  /* Vulkan 1.3.231 - A Specification, pg 2235 */

  auto surfaceCapabilities =
      vkh::getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface);
  auto surfaceFormats =
      vkh::getPhysicalDeviceSurfaceFormatList(physicalDevice, surface);
  auto surfacePresentModes =
      vkh::getPhysicalDeviceSurfacePresentModeList(physicalDevice, surface);

  VkSwapchainCreateInfoKHR swapchainCreateInfo{};
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = nullptr;

  /* Images of the swapchain can be used to create a VkImageView with a
  different format than what the swapchain was create with. */
  /* If flags contains VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR then the
  pNext chain must include a VkImageFormatListCreateInfo structure with a
  viewFormatCount greater than zero and pViewFormats must have an element
  equal to imageFormat */
  // swapchainCreateInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;

  swapchainCreateInfo.surface = surface;

  /* plus 1 to prevent waiting on the driver to complete internal operations
  before another image is accquired */
  uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

  /* minImageCount <= surfaceCapabilities.maxImageCount, and
  surfaceCapabilities.maxImageCount might be less than
  surfaceCapabilities.minImageCount */
  if (surfaceCapabilities.maxImageCount > 0 &&
      minImageCount > surfaceCapabilities.maxImageCount) {
    minImageCount = surfaceCapabilities.maxImageCount;
  }
  swapchainCreateInfo.minImageCount = minImageCount;

  /* imageFormat and imageColorSpace must match the surfaceFormats.format and
  surfaceFormats.colorSpace members, respectively */
  if (surfaceFormats.empty()) {
    throw std::runtime_error("No surface format available.");
  }
  swapchainCreateInfo.imageFormat = surfaceFormats[0].format;
  swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace;
  for (const auto &format : surfaceFormats) {
    if (format.format == VK_FORMAT_R8G8B8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      swapchainCreateInfo.imageFormat = format.format;
      swapchainCreateInfo.imageColorSpace = format.colorSpace;
      break;
    }
  }
  swapchain.format = swapchainCreateInfo.imageFormat;

  /* imageExtent is the size (in pixels) of the swapchain
  image(s). The behavior is platform-dependent if the image
  extent does not match the surface’s currentExtent as returned
  by vkGetPhysicalDeviceSurfaceCapabilitiesKHR. */
  uint32_t imageExtentWidth = surfaceCapabilities.currentExtent.width;
  uint32_t imageExtentHeight = surfaceCapabilities.currentExtent.height;

  /* {0, 0} <= minImageExtent <= imageExtent <= maxImageExtent */
  if (imageExtentWidth > surfaceCapabilities.maxImageExtent.width) {
    imageExtentWidth = surfaceCapabilities.maxImageExtent.width;
  }
  if (imageExtentHeight > surfaceCapabilities.maxImageExtent.height) {
    imageExtentHeight = surfaceCapabilities.maxImageExtent.height;
  }
  swapchainCreateInfo.imageExtent = {imageExtentWidth, imageExtentHeight};
  swapchain.extent = swapchainCreateInfo.imageExtent;

  /* imageArrayLayers is the number of views in a multiview/stereo surface. For
  non-stereoscopic-3D applications, this value is 1. */
  /* 0 < imageArrayLayers <= maxImageArrayLayers */
  swapchainCreateInfo.imageArrayLayers = 1;

  /* {presentMode} determines how incoming present requests will be processed
  and queued internally. */
  /* enum {VkPresentModeKHR}, pg 2219 */
  /* {VK_PRESENT_MODE_FIFO_KHR} is guaranteed to be available */
  swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  auto preferredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  for (const auto &presentMode : surfacePresentModes) {
    if (presentMode == preferredPresentMode) {
      swapchainCreateInfo.presentMode = preferredPresentMode;
      break;
    }
  }

  /* {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT} specifies that the image can be used
  to create a {VkImageView} suitable for use as a color or resolve attachment in
  a {VkFramebuffer}, pg 898 */
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  /* {VK_SHARING_MODE_EXCLUSIVE} requires some works to be done with ownership
   */
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;

  /* image content is presented without being transformed */
  if (surfaceCapabilities.supportedTransforms &
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR not supported.");
  }

  if (surfaceCapabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR not supported.");
  }

  swapchainCreateInfo.clipped = VK_TRUE;

  swapchainCreateInfo.pNext = nullptr;
  swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

  return swapchainCreateInfo;
}

void VulkanApp::createSwapchain() {
  auto swapchainCreateInfo = populateSwapchainCreateInfo();
  swapchain.self = vkh::createSwapchain(device, &swapchainCreateInfo);
}

void VulkanApp::createImageViews() {
  images = vkh::getSwapchainImages(device, swapchain.self);
  size_t imageCount = images.size();
  imageViews.resize(imageCount);
  for (size_t i = 0; i < imageCount; ++i) {
    VkImageViewCreateInfo imageViewInfo{};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.pNext = nullptr;
    // imageViewInfo.flags =
    imageViewInfo.image = images[i];

    /* treat images as 2D textures */
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

    imageViewInfo.format = swapchain.format;

    /* default mapping */
    imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    /* color aspect */
    imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    /* In stereographic 3D applications, create a swapchain with multiple
    layers before creating multiple image views for each images representing
    the views for the left and right eyes by accessing different layers */
    imageViewInfo.subresourceRange.baseMipLevel = 0;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.subresourceRange.baseArrayLayer = 0;
    imageViewInfo.subresourceRange.layerCount = 1;

    imageViews[i] = vkh::createImageView(device, &imageViewInfo);
  }
}

void VulkanApp::createGraphicsPipeline() {
  vkh::ShaderModuleWrapper shaderModule{};
  shaderModule.vertex =
      vkh::createShaderModule(device, "build/shaders/triangle_vert.spv");
  shaderModule.fragment =
      vkh::createShaderModule(device, "build/shaders/triangle_frag.spv");

  // std::vector<VkGraphicsPipelineCreateInfo>
  // graphicsPipelineCreateInfos{};

  /*  */
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = shaderModule.vertex;
  vertShaderStageInfo.pName = "main";
  vertShaderStageInfo.pSpecializationInfo = nullptr;

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = shaderModule.fragment;
  fragShaderStageInfo.pName = "main";
  fragShaderStageInfo.pSpecializationInfo = nullptr;

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  /*  */
  VkPipelineVertexInputStateCreateInfo vertInputInfo{};
  vertInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertInputInfo.vertexBindingDescriptionCount = 0;
  vertInputInfo.pVertexBindingDescriptions = nullptr;
  vertInputInfo.vertexAttributeDescriptionCount = 0;
  vertInputInfo.pVertexAttributeDescriptions = nullptr;

  /*  */
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
  inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  /*  */
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain.extent.width);
  viewport.height = static_cast<float>(swapchain.extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchain.extent;

  VkPipelineViewportStateCreateInfo viewportStateInfo{};
  viewportStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportStateInfo.viewportCount = 1;
  viewportStateInfo.pViewports = &viewport;
  viewportStateInfo.scissorCount = 1;
  viewportStateInfo.pScissors = &scissor;

  /*  */
  VkPipelineRasterizationStateCreateInfo rastStateInfo{};
  rastStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rastStateInfo.pNext = nullptr;
  rastStateInfo.depthClampEnable = VK_FALSE;
  rastStateInfo.rasterizerDiscardEnable = VK_FALSE;
  rastStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
  rastStateInfo.lineWidth = 1.0f;
  rastStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
  rastStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rastStateInfo.depthBiasEnable = VK_FALSE;
  rastStateInfo.depthBiasConstantFactor = 0.0f;
  rastStateInfo.depthBiasClamp = 0.0f;
  rastStateInfo.depthBiasSlopeFactor = 0.0f;

  /*  */
  VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
  multisampleStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampleStateInfo.sampleShadingEnable = VK_FALSE;
  multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampleStateInfo.minSampleShading = 1.0f;
  multisampleStateInfo.pSampleMask = nullptr;
  multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
  multisampleStateInfo.alphaToOneEnable = VK_FALSE;

  /* Depth and stencil testing */

  /* Color blending */
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_TRUE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlendState{};
  colorBlendState.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlendState.logicOpEnable = VK_FALSE;
  colorBlendState.logicOp = VK_LOGIC_OP_COPY;
  colorBlendState.attachmentCount = 1;
  colorBlendState.pAttachments = &colorBlendAttachment;
  colorBlendState.blendConstants[0] = 0.0f;
  colorBlendState.blendConstants[1] = 0.0f;
  colorBlendState.blendConstants[2] = 0.0f;
  colorBlendState.blendConstants[3] = 0.0f;

  /* Dynamic state */
  // std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
  //                                              VK_DYNAMIC_STATE_LINE_WIDTH};

  // VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
  // dynamicStateInfo.sType =
  //     VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  // dynamicStateInfo.dynamicStateCount =
  //     static_cast<uint32_t>(dynamicStates.size());
  // dynamicStateInfo.pDynamicStates = dynamicStates.data();

  /* Pipeline layout */
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  graphicsPipeline.layout =
      vkh::createPipelineLayout(device, &pipelineLayoutInfo);

  /* Render pass */
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swapchain.format;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  graphicsPipeline.renderPass = vkh::createRenderPass(device, &renderPassInfo);

  /* Create a graphics pipeline */
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;
  pipelineInfo.stageCount = vkh::ShaderModuleWrapper::stageCount;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportStateInfo;
  pipelineInfo.pRasterizationState = &rastStateInfo;
  pipelineInfo.pMultisampleState = &multisampleStateInfo;
  pipelineInfo.pDepthStencilState = nullptr;
  pipelineInfo.pColorBlendState = &colorBlendState;
  pipelineInfo.pDynamicState = nullptr;
  pipelineInfo.layout = graphicsPipeline.layout;
  pipelineInfo.renderPass = graphicsPipeline.renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  std::vector<VkGraphicsPipelineCreateInfo> pipelineInfoWrapper{pipelineInfo};

  graphicsPipeline.self =
      vkh::createPipelines(device, VK_NULL_HANDLE, pipelineInfoWrapper)[0];

  vkh::destroyShaderModule(device, shaderModule.vertex);
  vkh::destroyShaderModule(device, shaderModule.fragment);
}

void VulkanApp::createCommandPool() {
  VkCommandPoolCreateInfo cmdPoolInfo{};
  cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmdPoolInfo.pNext = nullptr;
  /* allows any command buffer allocated from a
  pool to be individually reset to the initial state; either by calling
  vkResetCommandBuffer, or via the implicit reset when calling
  vkBeginCommandBuffer. If this flag is not set on a pool, then
  vkResetCommandBuffer must not be called for any command buffer allocated
  from that pool. */
  cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  /* All command buffers allocated from this command pool must be
  submitted on queues from the same queue family. */
  cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;

  commandPool = vkh::createCommandPool(device, &cmdPoolInfo);
}

VulkanApp::VulkanApp() {
  createWindow();
  createInstance();
  createDebugMessenger();
  createSurface();
  createDevice();
  createSwapchain();
  createImageViews();
  createGraphicsPipeline();

  createCommandPool();
}

VulkanApp::~VulkanApp() {
  vkh::destroyCommandPool(device, commandPool);

  vkh::destroyPipeline(device, graphicsPipeline.self);
  vkh::destroyPipelineLayout(device, graphicsPipeline.layout);
  vkh::destroyRenderPass(device, graphicsPipeline.renderPass);
  for (auto &imageView : imageViews) {
    vkh::destroyImageView(device, imageView);
  }
  vkh::destroySwapchain(device, swapchain.self);
  vkh::destroyDevice(device);
  vkh::destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  vkh::destroySurface(instance, surface);
  vkh::destroyInstance(instance, nullptr);
  vkh::destroyWindow(window);
  glfwTerminate();
}

void VulkanApp::run() {
  vkh::Timer time_total;
  const double sec_to_mics = 1'000'000.0;
  while (!glfwWindowShouldClose(window)) {
    vkh::Timer time_circle;
    glfwPollEvents();

    // for (int i = 0; i < 1'000'000; ++i) {
    // }

    double current = static_cast<double>(time_total.current());
    if (current >= sec_to_mics) {
      std::cout << sec_to_mics / static_cast<double>(time_circle.current())
                << " FPS\n";
      time_total.reset();
    }
  }
  vkDeviceWaitIdle(device);
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
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
  auto selectedDeviceProperties =
      vkh::getPhysicalDevicePropertyList(physicalDevice);
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