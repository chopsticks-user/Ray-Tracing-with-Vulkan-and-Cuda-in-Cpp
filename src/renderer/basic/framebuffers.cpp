#include "framebuffers.hpp"

#include "logical_device.hpp"
#include "render_pass.hpp"
#include "swapchain.hpp"
#include "usage_specific.hpp"

#include <array>

namespace neko {

Framebuffers::Framebuffers(const Device &crDevice, const Swapchain &crSwapchain,
                           const RenderPass &crRenderPass,
                           const DepthBuffer &crDepthBuffer)
    : mpcDevice{&crDevice} {
  auto swapchainImageViews = crSwapchain.getImageViews();
  std::array<VkImageView, 2> attachments{crDepthBuffer.view(), {}};
  auto swapchainExtent = crSwapchain.extent();

  VkFramebufferCreateInfo framebufferInfo{};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = *crRenderPass;
  framebufferInfo.attachmentCount = vku32(attachments.size());
  framebufferInfo.width = swapchainExtent.width;
  framebufferInfo.height = swapchainExtent.height;
  framebufferInfo.layers = 1;

  for (u64 iImageView = 0; iImageView < swapchainImageViews.size();
       ++iImageView) {
    attachments[1] = swapchainImageViews[iImageView];
    framebufferInfo.pAttachments = attachments.data();
    if (vkCreateFramebuffer(*crDevice, &framebufferInfo, nullptr,
                            &mFramebuffers[iImageView]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffers");
    }
  }
}

Framebuffers::Framebuffers(Framebuffers &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mFramebuffers{std::exchange(
                                               rhs.mFramebuffers, {})} {}

Framebuffers &Framebuffers::operator=(Framebuffers &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mFramebuffers = std::exchange(rhs.mFramebuffers, {});
  return *this;
}

void Framebuffers::release() noexcept {
  if (!mFramebuffers.empty()) {
    for (auto &framebuffer : mFramebuffers) {
      vkDestroyFramebuffer(**mpcDevice, framebuffer, nullptr);
    }
    mFramebuffers.clear();
  }
}

} /* namespace neko */