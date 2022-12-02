#include "graphics_pipeline.hpp"

namespace rtvc {

void GraphicsPipeline::_initialize(const CustomArgs &args) {
  //   bool enableMSAA = args.nSamples != VK_SAMPLE_COUNT_1_BIT;
  auto vertShaderModule =
      _makeShaderModule(args.device.ref(), absoluteDirectory + args.vertPath);
  auto fragShaderModule =
      _makeShaderModule(args.device.ref(), absoluteDirectory + args.fragPath);

  /*  */
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";
  vertShaderStageInfo.pSpecializationInfo = nullptr;

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";
  fragShaderStageInfo.pSpecializationInfo = nullptr;

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  /*  */
  auto bindingDescription = vkw::Vertex::getBindingDescription();
  auto attributeDescriptions = vkw::Vertex::getAttributeDescriptions();

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
  viewport.width = static_cast<float>(args.swapchain.extent().width);
  viewport.height = static_cast<float>(args.swapchain.extent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = args.swapchain.extent();

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
  multisampleStateInfo.sampleShadingEnable = VK_TRUE;
  multisampleStateInfo.rasterizationSamples = args.nSamples;
  multisampleStateInfo.minSampleShading = 0.2f;
  multisampleStateInfo.pSampleMask = nullptr;
  multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
  multisampleStateInfo.alphaToOneEnable = VK_FALSE;

  /* Depth and stencil testing */
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
  depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencilInfo.depthTestEnable = VK_TRUE;
  depthStencilInfo.depthWriteEnable = VK_TRUE;
  depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  depthStencilInfo.minDepthBounds = 0.0f;
  depthStencilInfo.maxDepthBounds = 1.0f;
  depthStencilInfo.stencilTestEnable = VK_FALSE;
  depthStencilInfo.front = {};
  depthStencilInfo.back = {};

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
  pipelineLayoutInfo.pSetLayouts = &args.desSetLayout.ref();
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  vkCreatePipelineLayout(args.device.ref(), &pipelineLayoutInfo, nullptr,
                         &_pipelineLayout);

  /* Render pass */
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = args.swapchain.format();
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  colorAttachment.samples = args.nSamples;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = args.depthFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  depthAttachment.samples = args.nSamples;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachmentResolve{};
  colorAttachmentResolve.format = args.swapchain.format();
  colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentResolveRef{};
  colorAttachmentResolveRef.attachment = 2;
  colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  subpass.pResolveAttachments = &colorAttachmentResolveRef;

  VkSubpassDependency subPassDep{}; /* Needed when rendering */
  subPassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
  subPassDep.dstSubpass = 0;
  subPassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subPassDep.srcAccessMask = 0;
  subPassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subPassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 3> attachments = {
      colorAttachment,
      depthAttachment,
      colorAttachmentResolve,
  };

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &subPassDep;
  vkCreateRenderPass(args.device.ref(), &renderPassInfo, nullptr, &_renderPass);

  /* Create a graphics pipeline */
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;
  pipelineInfo.stageCount = vkw::ShaderModuleWrapper::stageCount;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportStateInfo;
  pipelineInfo.pRasterizationState = &rastStateInfo;
  pipelineInfo.pMultisampleState = &multisampleStateInfo;
  pipelineInfo.pDepthStencilState = &depthStencilInfo;
  pipelineInfo.pColorBlendState = &colorBlendState;
  pipelineInfo.pDynamicState = nullptr;
  pipelineInfo.layout = _pipelineLayout;
  pipelineInfo.renderPass = _renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  vkCreateGraphicsPipelines(args.device.ref(), VK_NULL_HANDLE, 1, &pipelineInfo,
                            nullptr, &_graphicsPipeline);
  vkDestroyShaderModule(args.device.ref(), vertShaderModule, nullptr);
  vkDestroyShaderModule(args.device.ref(), fragShaderModule, nullptr);
  _device = args.device.ref();
  _isOwner = true;
}

} /* namespace rtvc */