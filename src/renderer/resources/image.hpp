#ifndef NEKO_RENDERER_RESOURCES_IMAGE_HPP
#define NEKO_RENDERER_RESOURCES_IMAGE_HPP

#include "core.hpp"

namespace neko {

class Device;

class Image {
public:
  Image() = default;
  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  Image(const Device &crDevice, const VkImageCreateInfo *pcImageInfo,
        const VkImageViewCreateInfo *pcImageViewInfo = nullptr,
        const VkMemoryAllocateInfo *pcAllocInfo = nullptr,
        VkDeviceSize memoryOffset = 0);

  Image(Image &&rhs) noexcept;

  Image &operator=(Image &&rhs) noexcept;

  virtual ~Image() noexcept { release(); }

  const VkImage &operator*() const noexcept { return mImage; }

  void release() noexcept;

  const VkImageView &view() const noexcept { return mImageView; }

protected:
  const Device *mpcDevice = nullptr;
  VkImage mImage = VK_NULL_HANDLE;
  VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
  VkDeviceSize mOffset = 0;
  VkImageView mImageView = VK_NULL_HANDLE;
  bool mIsOwner = false;

  void createImage(const VkImageCreateInfo *pcImageInfo);

  void createImageMemory(const VkMemoryAllocateInfo *pcAllocInfo,
                         VkDeviceSize memoryOffset);

  void createImageView(const VkImageViewCreateInfo *pcImageViewInfo);
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_IMAGE_HPP */