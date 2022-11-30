#ifndef VKW_PIPELINE_GRAPHICS_HPP
#define VKW_PIPELINE_GRAPHICS_HPP

#include "config.hpp"

#include "structures/structures.hpp"

#include <vkh.hpp>

namespace vkw {

class GraphicsPipeline {
public:
  struct CustomArgs {
    VkDevice device;
    VkExtent2D extent;
    VkFormat format;
    const VkDescriptorSetLayout *desSetLayout;
    const std::string vertexFilePath;
    const std::string fragFilePath;
  };

  GraphicsPipeline() = default;
  GraphicsPipeline(const CustomArgs &args)
      : _device{args.device}, _pipelineCache{nullptr}, _pAllocator{nullptr} {
    _customInitialize(args);
  }
  GraphicsPipeline(VkDevice device,
                   const VkGraphicsPipelineCreateInfo *pCreateInfo,
                   VkPipelineCache pipelineCache = nullptr,
                   const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pipelineCache{pipelineCache},
        _pipelineLayout{pCreateInfo->layout},
        _renderPass{pCreateInfo->renderPass}, _pAllocator{pAllocator} {
    _graphicsPipeline =
        vkh::createPipeline(device, pipelineCache, pCreateInfo, pAllocator);
    _isOwner = true;
  }
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline(GraphicsPipeline &&rhs) { _moveDataFrom(std::move(rhs)); }
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~GraphicsPipeline() { _destroyVkData(); }

  const VkPipeline &ref() const noexcept { return _graphicsPipeline; }

  const VkPipelineCache &cache() const noexcept { return _pipelineCache; }

  const VkPipelineLayout &layout() const noexcept { return _pipelineLayout; }

  const VkRenderPass &renderPass() const noexcept { return _renderPass; }

protected:
  VkPipeline _graphicsPipeline;
  VkDevice _device;
  VkPipelineCache _pipelineCache;
  VkPipelineLayout _pipelineLayout;
  VkRenderPass _renderPass;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(GraphicsPipeline &&rhs) {
    _destroyVkData();
    _graphicsPipeline = rhs._graphicsPipeline;
    _device = rhs._device;
    _pipelineCache = rhs._pipelineCache;
    _pipelineLayout = rhs._pipelineLayout;
    _renderPass = rhs._renderPass;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyPipeline(_device, _graphicsPipeline, _pAllocator);
      vkh::destroyPipelineLayout(_device, _pipelineLayout, _pAllocator);
      vkh::destroyRenderPass(_device, _renderPass, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "GraphicsPipeline destructor" << '\n';
      }
    }
  }

private:
  void _customInitialize(const CustomArgs &args) {
    vkw::ShaderModuleWrapper shaderModule{};
    shaderModule.vertex = vkh::createShaderModule(
        args.device, vkh::absoluteDirectory + args.vertexFilePath);
    shaderModule.fragment = vkh::createShaderModule(
        args.device, vkh::absoluteDirectory + args.fragFilePath);

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
    viewport.width = static_cast<float>(args.extent.width);
    viewport.height = static_cast<float>(args.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = args.extent;

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
    pipelineLayoutInfo.pSetLayouts = args.desSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    _pipelineLayout =
        vkh::createPipelineLayout(args.device, &pipelineLayoutInfo);

    /* Render pass */
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = args.format;
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
    _renderPass = vkh::createRenderPass(args.device, &renderPassInfo);

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
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    _graphicsPipeline = vkh::createPipeline<vkh::Graphics>(
        args.device, _pipelineCache, &pipelineInfo);
    vkh::destroyShaderModule(args.device, shaderModule.vertex);
    vkh::destroyShaderModule(args.device, shaderModule.fragment);
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_PIPELINE_GRAPHICS_HPP */