#include "Framebuffers.hpp"
#include "Swapchain.hpp"

#include "Devices/Logical.hpp"
#include "Pipelines/RenderPass.hpp"
#include "Resources/UsageSpecific.hpp"

#include <array>

namespace Neko::Internal::VK
{

    Framebuffers::Framebuffers(const Device &crDevice, const Swapchain &crSwapchain,
                               const RenderPass &crRenderPass,
                               const DepthBuffer &crDepthBuffer)
        : mpcDevice{&crDevice}
    {
        auto swapchainImageViews = crSwapchain.getImageViews();
        std::array<VkImageView, 2> attachments{VkImageView{}, crDepthBuffer.view()};
        auto swapchainExtent = crSwapchain.extent();

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *crRenderPass;
        framebufferInfo.attachmentCount = vku32(attachments.size());
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        mFramebuffers.resize(swapchainImageViews.size());
        for (u64 iImageView = 0; iImageView < swapchainImageViews.size();
             ++iImageView)
        {
            attachments[0] = swapchainImageViews[iImageView];
            framebufferInfo.pAttachments = attachments.data();
            if (vkCreateFramebuffer(*crDevice, &framebufferInfo, nullptr,
                                    &mFramebuffers[iImageView]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffers");
            }
        }

        // TODO: keep imageViews alive till {Framebuffers} is destroyed
        // for (const auto &imageView : swapchainImageViews) {
        //   vkDestroyImageView(*crDevice, imageView, nullptr);
        // }
    }

    Framebuffers::Framebuffers(Framebuffers &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)}, mFramebuffers{
                                                   std::move(rhs.mFramebuffers)}
    {
        if (!rhs.mFramebuffers.empty())
        {
            rhs.mFramebuffers.clear();
        }
    }

    Framebuffers &Framebuffers::operator=(Framebuffers &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mFramebuffers = std::move(rhs.mFramebuffers);
        if (!rhs.mFramebuffers.empty())
        {
            rhs.mFramebuffers.clear();
        }
        return *this;
    }

    void Framebuffers::release() noexcept
    {
        if (!mFramebuffers.empty())
        {
            for (auto &framebuffer : mFramebuffers)
            {
                vkDestroyFramebuffer(**mpcDevice, framebuffer, nullptr);
            }
            mFramebuffers.clear();
        }
    }

} // namespace Neko::Internal::VK