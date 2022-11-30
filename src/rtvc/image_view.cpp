#include "image_view.hpp"

namespace rtvc {

uint32_t Image::_findMemoryType(VkPhysicalDevice physicalDevice,
                                uint32_t typeFilter,
                                VkMemoryPropertyFlags propFlags) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
    if (typeFilter & (1 << i) &&
        (memoryProperties.memoryTypes[i].propertyFlags & propFlags) ==
            propFlags) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory type.");
}

void Image::_initialize(VkDevice device, VkPhysicalDevice physicalDevice,
                        const CustomArgs &args) {
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = args.width;
  imageInfo.extent.height = args.height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = args.format;
  imageInfo.tiling = args.tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = args.usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0;
  if (vkCreateImage(device, &imageInfo, nullptr, &_image) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image.");
  }

  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(device, _image, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = _findMemoryType(
      physicalDevice, memoryRequirements.memoryTypeBits, args.propFlags);
  if (vkAllocateMemory(device, &allocInfo, nullptr, &_deviceMemory)) {
    throw std::runtime_error("Failed to allocate image memory.");
  }
  vkBindImageMemory(device, _image, _deviceMemory, 0);
  _device = device;
  _isOwner = true;
}

void ImageView::_initialize(VkDevice device, VkImage image, VkFormat format) {
  // auto images = vkh::getSwapchainImages(device, swapchain);
  // size_t imageCount = images.size();
  // _imageViews.resize(imageCount);
  VkImageViewCreateInfo imageViewInfo{};
  imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  imageViewInfo.pNext = nullptr;
  // imageViewInfo.flags =
  imageViewInfo.image = image;

  /* treat images as 2D textures */
  imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

  imageViewInfo.format = format;

  /* default mapping */
  imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  /* color aspect */
  imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

  /* In stereographic 3D applications, create a swapchain with multiple
  layers before creating multiple image views for each images representing
  the views for the left and right eyes by accessing different layers */
  imageViewInfo.subresourceRange.baseMipLevel = 0;
  imageViewInfo.subresourceRange.levelCount = 1;
  imageViewInfo.subresourceRange.baseArrayLayer = 0;
  imageViewInfo.subresourceRange.layerCount = 1;

  vkCreateImageView(device, &imageViewInfo, nullptr, &_imageView);
  _device = device;
  _isOwner = true;
}

} /* namespace rtvc */