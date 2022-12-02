#include "framebuffers.hpp"

namespace rtvc {

void Framebuffers::_initialize(
    VkDevice device, const std::vector<VkImageView> &swapchainImageViews,
    VkImageView depthView, VkImageView colorView, VkRenderPass renderPass,
    const VkExtent2D &extent) {
  size_t imageCount = swapchainImageViews.size();
  _framebuffers.resize(imageCount);
  for (size_t i = 0; i < imageCount; ++i) {
    std::array<VkImageView, 3> attachments{colorView, depthView,
                                           swapchainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;
    vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_framebuffers[i]);
  }
  _device = device;
  _isOwner = true;
}

} /* namespace rtvc */