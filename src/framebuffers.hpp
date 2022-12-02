#ifndef RTVC_FRAMEBUFFERS_HPP
#define RTVC_FRAMEBUFFERS_HPP

#include "device.hpp"
#include "graphics_pipeline.hpp"
#include "image_view.hpp"
#include "swapchain.hpp"

namespace rtvc {

class Framebuffers : public vkw::Framebuffers {
public:
  using vkw::Framebuffers::Framebuffers;

  Framebuffers(const Device &device,
               const SwapchainImageViews &swapchainImageViews,
               const ImageView &depthView, const ImageView &colorView,
               const GraphicsPipeline &graphicsPipeline,
               const Swapchain &swapchain) {
    _initialize(device.ref(), swapchainImageViews.ref(), depthView.ref(),
                colorView.ref(), graphicsPipeline.renderPass(),
                swapchain.extent());
  }

private:
  void _initialize(VkDevice device,
                   const std::vector<VkImageView> &swapchainImageViews,
                   VkImageView depthView, VkImageView colorView,
                   VkRenderPass renderPass, const VkExtent2D &extent);
};

} /* namespace rtvc */
#endif /* RTVC_FRAMEBUFFERS_HPP */