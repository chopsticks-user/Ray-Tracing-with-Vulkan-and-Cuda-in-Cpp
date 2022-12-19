#include "image.hpp"

#include "logical_device.hpp"

namespace neko {

ImageObject::ImageObject(const Device &crDevice,
                         const VkImageCreateInfo *pcImageInfo,
                         const VkImageViewCreateInfo *pcImageViewInfo,
                         const VkMemoryAllocateInfo *pcAllocInfo,
                         VkDeviceSize memoryOffset) {
  mpcDevice = &crDevice;
  createImage(pcImageInfo);
  createImageMemory(pcAllocInfo, memoryOffset);
  createImageView(pcImageViewInfo);
  mOffset = memoryOffset;
}

ImageObject::ImageObject(ImageObject &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mImage{std::exchange(
                                               rhs.mImage, VK_NULL_HANDLE)},
      mDeviceMemory{std::exchange(rhs.mDeviceMemory, VK_NULL_HANDLE)},
      mOffset{std::exchange(rhs.mOffset, false)},
      mImageView{std::exchange(rhs.mImageView, VK_NULL_HANDLE)} {}

ImageObject &ImageObject::operator=(ImageObject &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mImage = std::exchange(rhs.mImage, VK_NULL_HANDLE);
  mDeviceMemory = std::exchange(rhs.mDeviceMemory, VK_NULL_HANDLE);
  mOffset = std::exchange(rhs.mOffset, 0);
  mImageView = std::exchange(rhs.mImageView, VK_NULL_HANDLE);
  return *this;
}

void ImageObject::release() noexcept {
  if (mImage != VK_NULL_HANDLE) {
    if (mImageView != VK_NULL_HANDLE) {
      vkDestroyImageView(**mpcDevice, mImageView, nullptr);
      mImageView = VK_NULL_HANDLE;
    }
    if (mDeviceMemory != VK_NULL_HANDLE) {
      vkFreeMemory(**mpcDevice, mDeviceMemory, nullptr);
      mDeviceMemory = VK_NULL_HANDLE;
    }
    vkDestroyImage(**mpcDevice, mImage, nullptr);
    mImage = VK_NULL_HANDLE;
  }
}

void ImageObject::createImage(const VkImageCreateInfo *pcImageInfo) {
  if (vkCreateImage(**mpcDevice, pcImageInfo, nullptr, &mImage) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image");
  }
}

void ImageObject::createImageMemory(const VkMemoryAllocateInfo *pcAllocInfo,
                                    VkDeviceSize memoryOffset) {
  if (pcAllocInfo != nullptr) {
    if (vkAllocateMemory(**mpcDevice, pcAllocInfo, nullptr, &mDeviceMemory) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate image memory");
    }
    if (vkBindImageMemory(**mpcDevice, mImage, mDeviceMemory, memoryOffset) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to bind image to memory");
    }
  }
}

void ImageObject::createImageView(
    const VkImageViewCreateInfo *pcImageViewInfo) {
  if (pcImageViewInfo != nullptr &&
      vkCreateImageView(**mpcDevice, pcImageViewInfo, nullptr, &mImageView) !=
          VK_SUCCESS) {
    throw std::runtime_error("Failed to create image view");
  }
}

} /* namespace neko */