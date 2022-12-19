#include "image.hpp"

#include "logical_device.hpp"

namespace neko {

Image::Image(const Device &crDevice, const VkImageCreateInfo *pcImageInfo,
             const VkImageViewCreateInfo *pcImageViewInfo,
             const VkMemoryAllocateInfo *pcAllocInfo,
             VkDeviceSize memoryOffset) {
  mpcDevice = &crDevice;
  createImage(pcImageInfo);
  createImageMemory(pcAllocInfo, memoryOffset);
  createImageView(pcImageViewInfo);
  mOffset = memoryOffset;
  mIsOwner = true;
}

Image::Image(Image &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mImage{std::move(rhs.mImage)},
      mDeviceMemory{std::move(rhs.mDeviceMemory)}, mOffset{std::exchange(
                                                       rhs.mOffset, false)},
      mImageView{std::move(rhs.mImageView)}, mIsOwner{std::exchange(
                                                 rhs.mIsOwner, false)} {}

Image &Image::operator=(Image &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mImage = std::move(rhs.mImage);
  mDeviceMemory = std::move(rhs.mDeviceMemory);
  mOffset = std::exchange(rhs.mOffset, false);
  mImageView = std::move(rhs.mImageView);
  mIsOwner = std::exchange(rhs.mIsOwner, false);
  return *this;
}

void Image::release() noexcept {
  if (mIsOwner) {
    vkDestroyImageView(**mpcDevice, mImageView, nullptr);
    vkFreeMemory(**mpcDevice, mDeviceMemory, nullptr);
    vkDestroyImage(**mpcDevice, mImage, nullptr);
    mIsOwner = false;
  }
}

void Image::createImage(const VkImageCreateInfo *pcImageInfo) {
  if (vkCreateImage(**mpcDevice, pcImageInfo, nullptr, &mImage) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image");
  }
}

void Image::createImageMemory(const VkMemoryAllocateInfo *pcAllocInfo,
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

void Image::createImageView(const VkImageViewCreateInfo *pcImageViewInfo) {
  if (pcImageViewInfo != nullptr &&
      vkCreateImageView(**mpcDevice, pcImageViewInfo, nullptr, &mImageView) !=
          VK_SUCCESS) {
    throw std::runtime_error("Failed to create image view");
  }
}

} /* namespace neko */