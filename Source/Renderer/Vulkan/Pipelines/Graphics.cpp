#include "Graphics.hpp"
#include "PipelineCache.hpp"
#include "Layout/PipelineLayout.hpp"
#include "RenderPass/RenderPass.hpp"
#include "Shaders/ShaderModules.hpp"
#include "Shaders/ShaderObjects.hpp"

#include "Swapchain/Swapchain.hpp"
#include "Logical/Logical.hpp"
#include "Resources/Descriptors.hpp"
#include "Resources/UsageSpecific.hpp"

namespace Neko::Internal
{
    GraphicsPipeline::GraphicsPipeline(
        [[maybe_unused]] const EngineConfigs &crConfigs, const Device &crDevice,
        const Swapchain &crSwapchain, const RenderPass &crRenderPass,
        const PipelineLayout &crPipelineLayout,
        const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths)
        : mpcDevice{&crDevice}, mpcRenderPass{&crRenderPass},
          mpcPipelineLayout{&crPipelineLayout}
    {
        /* Shader stage */
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pName = "main";
        shaderStageInfo.pSpecializationInfo = nullptr;
        shaderStageInfo.pNext = nullptr;

        auto shaderModuleCount = pairShaderPaths.size();
        std::vector<VkPipelineShaderStageCreateInfo> shaderInfos{shaderModuleCount,
                                                                 shaderStageInfo};
        std::vector<ShaderModule> shaderModules{shaderModuleCount};
        for (u64 iShader = 0; iShader < shaderModuleCount; ++iShader)
        {
            const auto &[shaderStage, shaderPath] = pairShaderPaths[iShader];
            shaderModules[iShader] = {crDevice, shaderPath};
            shaderInfos[iShader].stage =
                static_cast<VkShaderStageFlagBits>(shaderStage);
            shaderInfos[iShader].module = *shaderModules[iShader];
        }

        /* Vertex input state */
        auto bindingDescription = ShaderObject::Vertex::getBindingDescription();
        auto attributeDescriptions =
            ShaderObject::Vertex::getAttributeDescriptions();

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
        //   multisampleInfo.sampleShadingEnable = VK_TRUE;
        //   multisampleInfo.minSampleShading = 0.2f;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
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

        /* Graphics pipeline */
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = nullptr;
        pipelineInfo.stageCount = vku32(shaderInfos.size());
        pipelineInfo.pStages = shaderInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &rasterizationInfo;
        pipelineInfo.pMultisampleState = &multisampleInfo;
        pipelineInfo.pDepthStencilState = &depthStencilInfo;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = **mpcPipelineLayout;
        pipelineInfo.renderPass = *crRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;
        if (vkCreateGraphicsPipelines(*crDevice, VK_NULL_HANDLE, 1, &pipelineInfo,
                                      nullptr, &mGPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline");
        }
    }

    GraphicsPipeline::GraphicsPipeline(GraphicsPipeline &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)},
          mpcRenderPass{std::move(rhs.mpcRenderPass)}, mpcPipelineLayout{std::move(
                                                           rhs.mpcPipelineLayout)},
          mGPipeline{std::exchange(rhs.mGPipeline, VK_NULL_HANDLE)} {}

    GraphicsPipeline &GraphicsPipeline::operator=(GraphicsPipeline &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mpcRenderPass = std::move(rhs.mpcRenderPass);
        mpcPipelineLayout = std::move(rhs.mpcPipelineLayout);
        mGPipeline = std::exchange(rhs.mGPipeline, VK_NULL_HANDLE);
        return *this;
    }

    void GraphicsPipeline::release() noexcept
    {
        if (mGPipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(**mpcDevice, mGPipeline, nullptr);
            mGPipeline = VK_NULL_HANDLE;
        }
    }

} // namespace Neko::Internal