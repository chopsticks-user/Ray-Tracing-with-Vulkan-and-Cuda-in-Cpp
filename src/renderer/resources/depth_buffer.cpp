#include "depth_buffer.hpp"

#include "logical_device.hpp"
#include "swapchain.hpp"
#include "utils.hpp"

namespace neko {

DepthBuffer::DepthBuffer([[maybe_unused]] const Settings &crSettings,
                         const Device &crDevice, const Swapchain &crSwapchain) {
  mpcDevice = &crDevice;

  static const u32 mipLevels = 1;
  static const VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

  VkFormat depthFormat = detail::findSupportedFormat(
      crDevice.physical(),
      {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = crSwapchain.extent().width;
  imageInfo.extent.height = crSwapchain.extent().height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = mipLevels;
  imageInfo.arrayLayers = 1;
  imageInfo.format = depthFormat;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = samples;
  imageInfo.flags = 0;
  createImage(&imageInfo);

  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(**mpcDevice, mImage, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = detail::findMemoryType(
      mpcDevice->physical(), memoryRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  createImageMemory(&allocInfo, 0);

  VkImageViewCreateInfo imageViewInfo{};
  imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  imageViewInfo.pNext = nullptr;
  imageViewInfo.image = mImage;

  /* treat images as 2D textures */
  imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

  imageViewInfo.format = depthFormat;

  /* default mapping */
  imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  /* color aspect */
  imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

  /* In stereographic 3D applications, create a swapchain with multiple
  layers before creating multiple image views for each images representing
  the views for the left and right eyes by accessing different layers */
  imageViewInfo.subresourceRange.baseMipLevel = 0;
  imageViewInfo.subresourceRange.levelCount = mipLevels;
  imageViewInfo.subresourceRange.baseArrayLayer = 0;
  imageViewInfo.subresourceRange.layerCount = 1;

  createImageView(&imageViewInfo);

  mOffset = 0;
  mIsOwner = true;
}

} /* namespace neko */