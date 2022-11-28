#ifndef VKW_IMAGE_HPP
#define VKW_IMAGE_HPP

#include "config.hpp"

#include "api/stb_image.hpp"
#include <vkh.hpp>

namespace vkw {

class Image {
public:
  Image() = default;
  Image(VkDevice device, const VkImageCreateInfo *pCreateInfo,
        const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    if (vkCreateImage(device, pCreateInfo, _pAllocator, &_image) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create image.");
    }
    _isOwner = true;
  }
  Image(const Image &) = delete;
  Image(Image &&rhs) { _moveDataFrom(std::move(rhs)); }
  Image &operator=(const Image &) = delete;
  Image &operator=(Image &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Image() { _destroyVkData(); }

  const VkImage &ref() const noexcept { return _image; }

private:
  VkImage _image;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Image &&rhs) {
    _image = rhs._image;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyImage(_device, _image, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Image destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_IMAGE_HPP */