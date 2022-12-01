#ifndef RTVC_IMAGE_VIEW_HPP
#define RTVC_IMAGE_VIEW_HPP

#include "device.hpp"
#include "swapchain.hpp"

namespace rtvc {

class Image : public vkw::Image {
public:
  struct CustomArgs {
    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags propFlags;
  };

  using vkw::Image::Image;

  Image(const Device &device, const CustomArgs &args) {
    _initialize(device.ref(), device.physical(), args);
  }

private:
  uint32_t _findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                           VkMemoryPropertyFlags propFlags);

  void _initialize(VkDevice device, VkPhysicalDevice physicalDevice,
                   const CustomArgs &args);
};

class ImageView : public vkw::ImageView {
public:
  using vkw::ImageView::ImageView;

  ImageView(const Device &device, const Image &image, VkFormat format,
            VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT) {
    _initialize(device.ref(), image.ref(), format, aspectFlags);
  }

private:
  void _initialize(VkDevice device, VkImage image, VkFormat format,
                   VkImageAspectFlags aspectFlags);
};

class SwapchainImageViews : public vkw::ImageViews {
public:
  using vkw::ImageViews::ImageViews;

  SwapchainImageViews(const Device &device, const Swapchain &swapchain) {
    _initialize(device.ref(), swapchain.ref(), swapchain.format());
  }

private:
  void _initialize(VkDevice device, VkSwapchainKHR swapchain, VkFormat format);
};

} /* namespace rtvc */
#endif /* RTVC_IMAGE_VIEW_HPP */