#include "vulkan_app.hpp"

void VulkanApp::createWindow() {
  // glfwInit();
  // window = Window{800, 600};
  glfwSetWindowUserPointer(window.ref(), this);
  glfwSetFramebufferSizeCallback(window.ref(), framebufferResizeCallback);
}

void VulkanApp::createInstance() {
  // VkApplicationInfo appInfo{};
  // appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  // appInfo.pApplicationName = "Vulkan Application";
  // appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  // appInfo.pEngineName = "No Engine";
  // appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  // appInfo.apiVersion = VK_API_VERSION_1_3;

  // auto requiredExtensions = vkh::getRequiredInstanceExtensionNameList();
  // for (auto &&extension : requiredExtensions) {
  //   instanceExtensions.emplace_back(extension);
  // }

  // auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};
  // vkh::populateDebugMessengerCreateInfo(debugCreateInfo);

  // VkInstanceCreateInfo instanceInfo;
  // instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  // instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT
  // *)&debugCreateInfo; instanceInfo.pApplicationInfo = &appInfo;
  // instanceInfo.enabledLayerCount =
  // static_cast<uint32_t>(instanceLayers.size());
  // instanceInfo.ppEnabledLayerNames = instanceLayers.data();
  // instanceInfo.enabledExtensionCount =
  //     static_cast<uint32_t>(instanceExtensions.size());
  // instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

  // // instance = vkh::createInstance(&instanceInfo);
  // instance = Instance{&instanceInfo};
}

void VulkanApp::createDebugMessenger() {
  // if (vkh::checkValidationLayerSupport() == false) {
  //   throw std::runtime_error("Validation layers are not supported.");
  // }
  // debugMessenger = vkh::createDebugMessenger(instance.ref());
  debugMessenger = DebugMessenger{instance.ref()};
}

void VulkanApp::createSurface() {
  surface = Surface{instance.ref(), window.ref()};
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
      vkh::getPresentQueueFamilyPropertyList(physDev, surface.ref());

  /* Find a queue that is capable of handling graphics, computing, and
  supporting presentation to the surface */
  /* any queue family with {VK_QUEUE_GRAPHICS_BIT} or {VK_QUEUE_COMPUTE_BIT}
  capabilities already implicitly support {VK_QUEUE_TRANSFER_BIT} operations */
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
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance.ref());
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

  device = Device{selectedPhysDev, &deviceCreateInfo};

  /* Get a queue handle */
  vkGetDeviceQueue(device.ref(), selectedIndex, 0, &queue);

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
      vkh::getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface.ref());
  auto surfaceFormats =
      vkh::getPhysicalDeviceSurfaceFormatList(physicalDevice, surface.ref());
  auto surfacePresentModes = vkh::getPhysicalDeviceSurfacePresentModeList(
      physicalDevice, surface.ref());

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

  swapchainCreateInfo.surface = surface.ref();

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
  // auto  = VK_PRESENT_MODE_IMMEDIATE_KHR;
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
  swapchain.self = vkh::createSwapchain(device.ref(), &swapchainCreateInfo);
}

void VulkanApp::createImageViews() {
  images = vkh::getSwapchainImages(device.ref(), swapchain.self);
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

    imageViews[i] = vkh::createImageView(device.ref(), &imageViewInfo);
  }
}

void VulkanApp::createGraphicsPipeline() {
  vkh::ShaderModuleWrapper shaderModule{};
  shaderModule.vertex = vkh::createShaderModule(
      device.ref(), vkh::absoluteDirectory + "/build/shaders/"
                                             "triangle_vert.spv");
  shaderModule.fragment = vkh::createShaderModule(
      device.ref(), vkh::absoluteDirectory + "/build/shaders/"
                                             "triangle_frag.spv");

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
  auto bindingDescription = vkh::Vertex2D_RGB::getBindingDescription();
  auto attributeDescriptions = vkh::Vertex2D_RGB::getAttributeDescriptions();

  VkPipelineVertexInputStateCreateInfo vertInputInfo{};
  vertInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertInputInfo.vertexBindingDescriptionCount = 1;
  vertInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
  rastStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  graphicsPipelineDeps.layout =
      vkh::createPipelineLayout(device.ref(), &pipelineLayoutInfo);

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

  VkSubpassDependency subPassDep{}; /* Needed when rendering */
  subPassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
  subPassDep.dstSubpass = 0;
  subPassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subPassDep.srcAccessMask = 0;
  subPassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subPassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &subPassDep;
  graphicsPipelineDeps.renderPass =
      vkh::createRenderPass(device.ref(), &renderPassInfo);

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
  pipelineInfo.layout = graphicsPipelineDeps.layout;
  pipelineInfo.renderPass = graphicsPipelineDeps.renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  graphicsPipeline = Pipeline<vkh::Graphics>{
      device.ref(), graphicsPipelineDeps.cache, &pipelineInfo};

  vkh::destroyShaderModule(device.ref(), shaderModule.vertex);
  vkh::destroyShaderModule(device.ref(), shaderModule.fragment);
}

void VulkanApp::createFramebuffers() {
  std::vector<VkFramebufferCreateInfo> framebufferInfos{imageViews.size()};
  std::vector<VkImageView> attachments{imageViews};
  for (size_t i = 0; i < imageViews.size(); ++i) {
    framebufferInfos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfos[i].renderPass = graphicsPipelineDeps.renderPass;
    framebufferInfos[i].attachmentCount = 1;
    framebufferInfos[i].pAttachments = &attachments[i];
    framebufferInfos[i].width = swapchain.extent.width;
    framebufferInfos[i].height = swapchain.extent.height;
    framebufferInfos[i].layers = 1;
  }
  framebuffers = Framebuffers{device.ref(), framebufferInfos};
}

void VulkanApp::recreateSwapchain() {
  /* Handle minimization */
  int width = 0, heigth = 0;
  glfwGetFramebufferSize(window.ref(), &width, &heigth);
  while (width == 0 || heigth == 0) {
    glfwGetFramebufferSize(window.ref(), &width, &heigth);
    glfwWaitEvents();
  }

  /* Recreate the swapchain */
  vkDeviceWaitIdle(device.ref());
  cleanupSwapchain();
  createSwapchain();
  createImageViews();
  createGraphicsPipeline();
  createFramebuffers();
  // createUniformBuffers();
}

void VulkanApp::cleanupSwapchain() {
  // for (size_t i = 0; i < maxFramesInFlight; ++i) {
  //   vkDestroyBuffer(device, uniformBuffers[i], nullptr);
  //   vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
  // }
  // for (auto &framebuffer : framebuffers) {
  //   vkh::destroyFramebuffer(device.ref(), framebuffer);
  // }
  // vkh::destroyPipeline(device.ref(), graphicsPipeline.ref());
  vkh::destroyPipelineLayout(device.ref(), graphicsPipelineDeps.layout);
  vkh::destroyRenderPass(device.ref(), graphicsPipelineDeps.renderPass);
  for (auto &imageView : imageViews) {
    vkh::destroyImageView(device.ref(), imageView);
  }
  vkh::destroySwapchain(device.ref(), swapchain.self);
}

void VulkanApp::framebufferResizeCallback(GLFWwindow *windowInstance,
                                          [[maybe_unused]] int width,
                                          [[maybe_unused]] int height) {
  auto app =
      reinterpret_cast<VulkanApp *>(glfwGetWindowUserPointer(windowInstance));
  app->framebufferResized = true;
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

  commandPool = vkh::createCommandPool(device.ref(), &cmdPoolInfo);
}

void VulkanApp::createCommandBuffers() {
  VkCommandBufferAllocateInfo cmdBufferAllocInfo{};
  cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufferAllocInfo.commandPool = commandPool;
  cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufferAllocInfo.commandBufferCount =
      static_cast<uint32_t>(maxFramesInFlight);
  commandBuffers =
      vkh::allocateCommandBuffers(device.ref(), &cmdBufferAllocInfo);
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
  renderPassBeginInfo.renderPass = graphicsPipelineDeps.renderPass;
  renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
  renderPassBeginInfo.renderArea.offset = {0, 0};
  renderPassBeginInfo.renderArea.extent = swapchain.extent;
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  /* 3. Drawing commands */
  vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline.ref());

  /* Bind the buffers */
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

  /* Bind the descriptor sets */
  vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphicsPipelineDeps.layout, 0, 1,
                          descriptorSets.data(), 0, nullptr);

  /* Bind if using index buffers */
  vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

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
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
    if (typeFilter & (1 << i) &&
        (memoryProperties.memoryTypes[i].propertyFlags & propFlags) ==
            propFlags) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type.");
}

void VulkanApp::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                             VkMemoryPropertyFlags propertyFlags,
                             VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
  [[maybe_unused]] static constexpr uint32_t vertexBufferCount = 1;
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device.ref(), &bufferInfo, nullptr, &buffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed creating vertex buffer.");
  }

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device.ref(), buffer, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memoryRequirements.memoryTypeBits, propertyFlags);
  if (vkAllocateMemory(device.ref(), &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate vertex buffer memory.");
  }
  vkBindBufferMemory(device.ref(), buffer, bufferMemory, 0);
}

void VulkanApp::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer =
      vkh::allocateCommandBuffer(device.ref(), &allocInfo);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkh::beginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

  vkh::endCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  /* Submit and wait on this transfer to complete before cleaning up
  the command buffer */
  vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(queue);
  vkFreeCommandBuffers(device.ref(), commandPool, 1, &commandBuffer);
}

void VulkanApp::createVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(shader::triangle_index_data[0]) *
                            shader::triangle_index_data.size();

  /* Create a host visible buffer as temporary buffer */
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);

  /* Copy the vertices data to the staging buffer */
  void *data;
  vkMapMemory(device.ref(), stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, shader::triangle_index_data.data(),
              static_cast<size_t>(bufferSize));
  vkUnmapMemory(device.ref(), stagingBufferMemory);

  /* Create a device local buffer as actual vertex buffer */
  createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

  /* Copy the data from the staging buffer to the device buffer */
  copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

  /* Clean the staging buffer */
  vkDestroyBuffer(device.ref(), stagingBuffer, nullptr);
  vkFreeMemory(device.ref(), stagingBufferMemory, nullptr);
}

void VulkanApp::createIndexBuffer() {
  VkDeviceSize bufferSize =
      sizeof(shader::triangle_indices[0]) * shader::triangle_indices.size();

  /* Create a host visible buffer as temporary buffer */
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);

  /* Copy the vertices data to the staging buffer */
  void *data;
  vkMapMemory(device.ref(), stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, shader::triangle_indices.data(),
              static_cast<size_t>(bufferSize));
  vkUnmapMemory(device.ref(), stagingBufferMemory);

  /* Create a device local buffer as actual vertex buffer */
  createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

  /* Copy the data from the staging buffer to the device buffer */
  copyBuffer(stagingBuffer, indexBuffer, bufferSize);

  /* Clean the staging buffer */
  vkDestroyBuffer(device.ref(), stagingBuffer, nullptr);
  vkFreeMemory(device.ref(), stagingBufferMemory, nullptr);
}

void VulkanApp::createUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(vkh::UniformBufferObject);
  uniformBuffers.resize(maxFramesInFlight);
  uniformBuffersMemory.resize(maxFramesInFlight);
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 uniformBuffers[i], uniformBuffersMemory[i]);
  }
}

void VulkanApp::updateUniformBuffer(uint32_t currentImage) {
  static const auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  vkh::UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(180.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),
                              static_cast<float>(swapchain.extent.width) /
                                  swapchain.extent.height,
                              0.1f, 10.0f);
  ubo.proj[1][1] *= -1;

  void *data;
  vkMapMemory(device.ref(), uniformBuffersMemory[currentImage], 0, sizeof(ubo),
              0, &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(device.ref(), uniformBuffersMemory[currentImage]);
}

void VulkanApp::createSynchronizationObjects() {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  sync.imageAvailableSemaphore.resize(maxFramesInFlight);
  sync.renderFinisedSemaphore.resize(maxFramesInFlight);
  sync.inFlightFence.resize(maxFramesInFlight);
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    sync.imageAvailableSemaphore[i] =
        vkh::createSemaphore(device.ref(), &semaphoreInfo);
    sync.renderFinisedSemaphore[i] =
        vkh::createSemaphore(device.ref(), &semaphoreInfo);
    sync.inFlightFence[i] = vkh::createFence(device.ref(), &fenceInfo);
  }
}

void VulkanApp::createDescriptorSetLayout() {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings = &uboLayoutBinding;
  if (vkCreateDescriptorSetLayout(device.ref(), &layoutInfo, nullptr,
                                  &descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout.");
  }
}

void VulkanApp::createDescriptorPool() {
  VkDescriptorPoolSize descriptorPoolSize{};
  descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorPoolSize.descriptorCount = static_cast<uint32_t>(maxFramesInFlight);

  VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = 1;
  descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
  descriptorPoolInfo.maxSets = static_cast<uint32_t>(maxFramesInFlight);

  if (vkCreateDescriptorPool(device.ref(), &descriptorPoolInfo, nullptr,
                             &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool.");
  }
}

void VulkanApp::createDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts{maxFramesInFlight,
                                             descriptorSetLayout};
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.descriptorSetCount = static_cast<uint32_t>(maxFramesInFlight);
  allocInfo.pSetLayouts = layouts.data();

  descriptorSets.resize(maxFramesInFlight);
  if (vkAllocateDescriptorSets(device.ref(), &allocInfo,
                               descriptorSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets.");
  }

  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(vkh::UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(device.ref(), 1, &descriptorWrite, 0, nullptr);
  }
}

void VulkanApp::createTextureImage() {
  //
}

void VulkanApp::render() {
  /* 1. Wait for the previous frame to finish */
  vkWaitForFences(device.ref(), 1, &sync.inFlightFence[sync.currentFrame],
                  VK_TRUE, UINT64_MAX);

  /* 2. Acquire an image frome the swapchain */
  uint32_t imageIndex;
  VkResult result =
      vkAcquireNextImageKHR(device.ref(), swapchain.self, UINT64_MAX,
                            sync.imageAvailableSemaphore[sync.currentFrame],
                            VK_NULL_HANDLE, &imageIndex);
  if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapchain();
    return;
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to acquire swapchain image.");
  }
  vkResetFences(device.ref(), 1, &sync.inFlightFence[sync.currentFrame]);

  /* 3. Record a command buffer which draws the scene onto that image */
  vkResetCommandBuffer(commandBuffers[sync.currentFrame], 0);
  recordCommandBuffer(commandBuffers[sync.currentFrame], imageIndex);

  /* 4. Submit the recorded command buffer */
  static const uint32_t submitCount = 1;
  std::vector<VkSemaphore> waitSemaphores = {
      sync.imageAvailableSemaphore[sync.currentFrame]};
  std::vector<VkPipelineStageFlags> waitStages = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  std::vector<VkSemaphore> signalSemaphores = {
      sync.renderFinisedSemaphore[sync.currentFrame]};

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

  if (vkQueueSubmit(queue, submitCount, &submitInfo,
                    sync.inFlightFence[sync.currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer.");
  }

  /* 5. Present the swapchain image */
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount =
      static_cast<uint32_t>(signalSemaphores.size());
  presentInfo.pWaitSemaphores = signalSemaphores.data();
  presentInfo.swapchainCount = swapchainCount;
  presentInfo.pSwapchains = &swapchain.self;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(queue, &presentInfo);
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
  createWindow();
  createInstance();
  createDebugMessenger();
  createSurface();
  createDevice();
  createSwapchain();
  createImageViews();
  createDescriptorSetLayout();
  createGraphicsPipeline();
  createFramebuffers();
  createCommandPool();
  createVertexBuffer();
  createIndexBuffer();
  createUniformBuffers();
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
  createSynchronizationObjects();
  createTextureImage();
}

VulkanApp::~VulkanApp() {
  cleanupSwapchain();
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    vkDestroyBuffer(device.ref(), uniformBuffers[i], nullptr);
    vkFreeMemory(device.ref(), uniformBuffersMemory[i], nullptr);
  }
  vkDestroyDescriptorPool(device.ref(), descriptorPool, nullptr);
  vkDestroyDescriptorSetLayout(device.ref(), descriptorSetLayout, nullptr);
  vkDestroyBuffer(device.ref(), indexBuffer, nullptr);
  vkFreeMemory(device.ref(), indexBufferMemory, nullptr);
  vkDestroyBuffer(device.ref(), vertexBuffer, nullptr);
  vkFreeMemory(device.ref(), vertexBufferMemory, nullptr);
  for (size_t i = 0; i < maxFramesInFlight; ++i) {
    vkh::destroySemaphore(device.ref(), sync.imageAvailableSemaphore[i]);
    vkh::destroySemaphore(device.ref(), sync.renderFinisedSemaphore[i]);
    vkh::destroyFence(device.ref(), sync.inFlightFence[i]);
  }
  vkh::destroyCommandPool(device.ref(), commandPool);
  // vkh::destroyDevice(device.ref());
  // vkh::destroyDebugUtilsMessengerEXT(instance.ref(), debugMessenger,
  // nullptr); vkh::destroySurface(instance.ref(), surface);
  // vkh::destroyInstance(instance, nullptr);
  // vkh::destroyWindow(window.ref());
  // glfwTerminate();
}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window.ref())) {
    glfwPollEvents();
    render();
  }
  vkDeviceWaitIdle(device.ref());
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