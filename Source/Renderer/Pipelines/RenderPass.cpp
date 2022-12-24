#include "RenderPass.hpp"

#include "Basic/Swapchain.hpp"
#include "Devices/LogicalDevice.hpp"
#include "Resources/UsageSpecific.hpp"

#include <array>

namespace Neko
{

    RenderPass::RenderPass(const Device &crDevice,
                           const VkRenderPassCreateInfo &crRenderPassInfo)
        : mpcDevice{&crDevice}
    {
        if (vkCreateRenderPass(*crDevice, &crRenderPassInfo, nullptr, &mRenderPass) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass");
        }
    }

    RenderPass::RenderPass(const Device &crDevice, const Swapchain &crSwapchain,
                           const DepthBuffer &crDepthBuffer)
        : mpcDevice{&crDevice}
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = crSwapchain.format();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        // colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = crDepthBuffer.format();
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
        if (vkCreateRenderPass(*crDevice, &renderPassInfo, nullptr, &mRenderPass) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass");
        }
    }

    RenderPass::RenderPass(RenderPass &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)},
          mRenderPass{std::exchange(rhs.mRenderPass, VK_NULL_HANDLE)} {}

    RenderPass &RenderPass::operator=(RenderPass &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mRenderPass = std::exchange(rhs.mRenderPass, VK_NULL_HANDLE);
        return *this;
    }

    void RenderPass::release() noexcept
    {
        if (mRenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(**mpcDevice, mRenderPass, nullptr);
            mRenderPass = VK_NULL_HANDLE;
        }
    }

} // namespace Neko