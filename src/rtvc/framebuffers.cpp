#include "framebuffers.hpp"

namespace rtvc {

void Framebuffers::_initialize(
    VkDevice device, const std::vector<VkImageView> &swapchainImageViews,
    VkRenderPass renderPass, const VkExtent2D &extent) {
  size_t imageCount = swapchainImageViews.size();
  _framebuffers.resize(imageCount);
  std::vector<VkImageView> attachments{swapchainImageViews};
  for (size_t i = 0; i < imageCount; ++i) {
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &attachments[i];
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;
    vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_framebuffers[i]);
  }
  _device = device;
  _isOwner = true;
}

} /* namespace rtvc */