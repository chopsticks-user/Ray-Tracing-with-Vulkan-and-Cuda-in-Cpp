#ifndef RTVC_IMAGE_VIEW_HPP
#define RTVC_IMAGE_VIEW_HPP

#include "device.hpp"

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

class ImageView : public vkw::ImageView {};

class ImageViews : public vkw::ImageViews {};

} /* namespace rtvc */
#endif /* RTVC_IMAGE_VIEW_HPP */