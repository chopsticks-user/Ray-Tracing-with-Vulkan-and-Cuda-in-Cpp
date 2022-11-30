#ifndef VKW_RESOURCE_IMAGE_HPP
#define VKW_RESOURCE_IMAGE_HPP

#include "config.hpp"

#include "interface/stb_image.hpp"
#include <vkh.hpp>

namespace vkw {

class Image {
public:
  Image() = default;
  Image(VkDevice device, const VkImageCreateInfo *pCreateInfo,
        const VkMemoryAllocateInfo *pAllocInfo,
        const VkAllocationCallbacks *pImageAllocator = nullptr,
        const VkAllocationCallbacks *pMemoryAllocator = nullptr)
      : _device{device}, _pImageAllocator{pImageAllocator},
        _pMemoryAllocator{pMemoryAllocator} {
    _createAllocatedImage(pCreateInfo, pAllocInfo);
    vkBindImageMemory(_device, _image, _deviceMemory, 0);
    _isOwner = true;
  }
  Image(VkDevice device, VkDeviceSize memoryOffset,
        const VkImageCreateInfo *pCreateInfo,
        const VkMemoryAllocateInfo *pAllocInfo,
        const VkAllocationCallbacks *pImageAllocator = nullptr,
        const VkAllocationCallbacks *pMemoryAllocator = nullptr)
      : _device{device}, _memoryOffset{memoryOffset},
        _pImageAllocator{pImageAllocator}, _pMemoryAllocator{pMemoryAllocator} {
    _createAllocatedImage(pCreateInfo, pAllocInfo);
    vkBindImageMemory(_device, _image, _deviceMemory, memoryOffset);
    _isOwner = true;
  }
  Image(const Image &) = delete;
  Image(Image &&rhs) { _moveDataFrom(std::move(rhs)); }
  Image &operator=(const Image &) = delete;
  Image &operator=(Image &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Image() { _destroyVkData(); }

  const VkImage &ref() const noexcept { return _image; }

  const VkDeviceMemory &memory() const noexcept { return _deviceMemory; }

  const VkDeviceSize &offset() const noexcept { return _memoryOffset; }

protected:
  VkImage _image = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  VkDeviceMemory _deviceMemory = VK_NULL_HANDLE;
  VkDeviceSize _memoryOffset = 0;
  const VkAllocationCallbacks *_pImageAllocator = nullptr;
  const VkAllocationCallbacks *_pMemoryAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Image &&rhs) {
    _image = rhs._image;
    _device = rhs._device;
    _deviceMemory = rhs._deviceMemory;
    _memoryOffset = rhs._memoryOffset;
    _pImageAllocator = rhs._pImageAllocator;
    _pMemoryAllocator = rhs._pMemoryAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyImage(_device, _image, _pImageAllocator);
      vkFreeMemory(_device, _deviceMemory, _pMemoryAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Image destructor" << '\n';
      }
    }
  }

  void _createAllocatedImage(const VkImageCreateInfo *pCreateInfo,
                             const VkMemoryAllocateInfo *pAllocInfo) {
    if (vkCreateImage(_device, pCreateInfo, _pImageAllocator, &_image) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create image.");
    }
    if (vkAllocateMemory(_device, pAllocInfo, _pMemoryAllocator,
                         &_deviceMemory) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate image memory.");
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_RESOURCE_IMAGE_HPP */