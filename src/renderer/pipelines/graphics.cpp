#include "graphics.hpp"

#include "descriptor.hpp"
#include "logical_device.hpp"
#include "shader_objects.hpp"
#include "swapchain.hpp"
#include "usage_specific.hpp"

namespace neko {

GraphicsPipeline::GraphicsPipeline(
    [[maybe_unused]] const Configs &crConfigs, const Device &crDevice,
    const Swapchain &crSwapchain, const DescriptorSetLayout &crDSLayout,
    const DepthBuffer &depthBuffer,
    const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths,
    const PipelineCache &crCache)
    : mpcDevice{&crDevice}, mpcCache{crCache.empty() ? nullptr : &crCache} {
  /* Shader stage */
  VkPipelineShaderStageCreateInfo shaderStageInfo{};
  shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStageInfo.pName = "main";
  shaderStageInfo.pSpecializationInfo = nullptr;
  shaderStageInfo.pNext = nullptr;

  std::vector<VkPipelineShaderStageCreateInfo> shaderModules{
      pairShaderPaths.size(), shaderStageInfo};
  for (u64 iShader = 0; iShader < pairShaderPaths.size(); ++iShader) {
    const auto &[shaderStage, shaderPath] = pairShaderPaths[iShader];
    ShaderModule shaderModule = {crDevice, shaderPath};
    shaderModules[iShader].stage =
        static_cast<VkShaderStageFlagBits>(shaderStage);
    shaderModules[iShader].module = *shaderModule;
  }

  /* Vertex input state */
  auto bindingDescription = shader_object::Vertex::getBindingDescription();
  auto attributeDescriptions =
      shader_object::Vertex::getAttributeDescriptions();

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  /* Input assembly state */
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
  inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  /* Tessellation state */
  //   VkPipelineTessellationStateCreateInfo tessellationInfo{};

  /* Viewport state */
  auto swapchainExtent = crSwapchain.extent();

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchainExtent.width);
  viewport.height = static_cast<float>(swapchainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchainExtent;

  VkPipelineViewportStateCreateInfo viewportInfo{};
  viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportInfo.viewportCount = 1;
  viewportInfo.pViewports = &viewport;
  viewportInfo.scissorCount = 1;
  viewportInfo.pScissors = &scissor;

  /* Rasterization state */
  VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
  rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizationInfo.pNext = nullptr;
  rasterizationInfo.depthClampEnable = VK_FALSE;
  rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizationInfo.lineWidth = 1.0f;
  rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizationInfo.depthBiasEnable = VK_FALSE;
  rasterizationInfo.depthBiasConstantFactor = 0.0f;
  rasterizationInfo.depthBiasClamp = 0.0f;
  rasterizationInfo.depthBiasSlopeFactor = 0.0f;

  /* Multisample state */
  VkPipelineMultisampleStateCreateInfo multisampleInfo{};
  multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampleInfo.sampleShadingEnable = VK_TRUE;
  multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampleInfo.minSampleShading = 0.2f;
  multisampleInfo.pSampleMask = nullptr;
  multisampleInfo.alphaToCoverageEnable = VK_FALSE;
  multisampleInfo.alphaToOneEnable = VK_FALSE;

  /* Depth stencil state */
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

  /* Color blend state */
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
  //   std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
  //                                                VK_DYNAMIC_STATE_LINE_WIDTH};

  //   VkPipelineDynamicStateCreateInfo dynamicInfo{};
  //   dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  //   dynamicInfo.dynamicStateCount =
  //   static_cast<uint32_t>(dynamicStates.size()); dynamicInfo.pDynamicStates =
  //   dynamicStates.data();

  /* Render pass */
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = crSwapchain.format();
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = depthBuffer.format();
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  //   VkAttachmentDescription colorAttachmentResolve{};
  //   colorAttachmentResolve.format = args.swapchain.format();
  //   colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  //   colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  //   colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  //   colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  //   colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  //   colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  //   colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  //   VkAttachmentReference colorAttachmentResolveRef{};
  //   colorAttachmentResolveRef.attachment = 2;
  //   colorAttachmentResolveRef.layout =
  //   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  //   subpass.pResolveAttachments = &colorAttachmentResolveRef;

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

  std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                        depthAttachment};

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &subPassDep;
  mRenderPass = {crDevice, renderPassInfo};

  /* Pipeline layout */
  mLayout = {crDevice, {*crDSLayout}};

  /* Graphics pipeline */
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;
  pipelineInfo.stageCount = vku32(pairShaderPaths.size());
  pipelineInfo.pStages = &shaderStageInfo;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportInfo;
  pipelineInfo.pRasterizationState = &rasterizationInfo;
  pipelineInfo.pMultisampleState = &multisampleInfo;
  pipelineInfo.pDepthStencilState = &depthStencilInfo;
  pipelineInfo.pColorBlendState = &colorBlendState;
  pipelineInfo.pDynamicState = nullptr;
  pipelineInfo.layout = *mLayout;
  pipelineInfo.renderPass = *mRenderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;
  if (vkCreateGraphicsPipelines(
          *crDevice, mpcCache == nullptr ? VK_NULL_HANDLE : **mpcCache, 1,
          &pipelineInfo, nullptr, &mGPipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline");
  }
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mpcCache{std::move(rhs.mpcCache)},
      mGPipeline{std::exchange(rhs.mGPipeline, VK_NULL_HANDLE)},
      mLayout{std::move(rhs.mLayout)}, mRenderPass{std::move(rhs.mRenderPass)} {
}

GraphicsPipeline &GraphicsPipeline::operator=(GraphicsPipeline &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mpcCache = std::move(rhs.mpcCache);
  mGPipeline = std::exchange(rhs.mGPipeline, VK_NULL_HANDLE);
  mLayout = std::move(rhs.mLayout);
  mRenderPass = std::move(rhs.mRenderPass);
  return *this;
}

void GraphicsPipeline::release() noexcept {
  if (mGPipeline != VK_NULL_HANDLE) {
    vkDestroyPipeline(**mpcDevice, mGPipeline, nullptr);
    mGPipeline = VK_NULL_HANDLE;
  }
}

} /* namespace neko */