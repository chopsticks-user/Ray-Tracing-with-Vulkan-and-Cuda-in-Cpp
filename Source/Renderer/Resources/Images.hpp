#ifndef NEKO_RENDERER_RESOURCES_IMAGE_HPP
#define NEKO_RENDERER_RESOURCES_IMAGE_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;

class Image {
public:
  Image() = default;
  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

private:
};

class ImageView {
public:
  ImageView() = default;
  ImageView(const ImageView &) = delete;
  ImageView &operator=(const ImageView &) = delete;

private:
};

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

  const VkImage &operator*() const noexcept { return mImage; }

  virtual ~ImageObject() noexcept { release(); }

  const VkImage &image() const noexcept { return mImage; }

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
                         VkDeviceSize memoryOffset = 0);

  void createImageView(const VkImageViewCreateInfo *pcImageViewInfo);
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_IMAGE_HPP */