#ifndef NEKO_RENDERER_RESOURCES_IMAGE_HPP
#define NEKO_RENDERER_RESOURCES_IMAGE_HPP

#include "core.hpp"

namespace neko {

class Device;
class ImageObject {
public:
  ImageObject() = default;
  ImageObject(const ImageObject &) = delete;
  ImageObject &operator=(const ImageObject &) = delete;

  ImageObject(const Device &crDevice, const VkImageCreateInfo *pcImageInfo,
              const VkImageViewCreateInfo *pcImageViewInfo = nullptr,
              const VkMemoryAllocateInfo *pcAllocInfo = nullptr,
              VkDeviceSize memoryOffset = 0);

  ImageObject(ImageObject &&rhs) noexcept;

  ImageObject &operator=(ImageObject &&rhs) noexcept;

  virtual ~ImageObject() noexcept { release(); }

  const VkImage &operator*() const noexcept { return mImage; }

  const VkImageView &view() const noexcept { return mImageView; }

  void release() noexcept;

protected:
  const Device *mpcDevice = nullptr;
  VkImage mImage = VK_NULL_HANDLE;
  VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
  VkDeviceSize mOffset = 0;
  VkImageView mImageView = VK_NULL_HANDLE;

  void createImage(const VkImageCreateInfo *pcImageInfo);

  void createImageMemory(const VkMemoryAllocateInfo *pcAllocInfo,
                         VkDeviceSize memoryOffset);

  void createImageView(const VkImageViewCreateInfo *pcImageViewInfo);
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_IMAGE_HPP */