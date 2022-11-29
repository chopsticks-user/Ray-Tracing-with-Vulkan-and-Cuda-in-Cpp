#ifndef VKW_RESOURCE_IMAGE_HPP
#define VKW_RESOURCE_IMAGE_HPP

#include "config.hpp"

#include "interface/stb_image.hpp"
#include <vkh.hpp>

namespace vkw {

class Image {
public:
  struct CustomArgs {
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags propFlags;
  };

  Image() = default;
  Image(const CustomArgs &args) : _device{args.device} {
    _customInitialize(args);
  }
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
  VkImage _image;
  VkDevice _device;
  VkDeviceMemory _deviceMemory;
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
  void _customInitialize(const CustomArgs &args) {
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
    if (vkCreateImage(args.device, &imageInfo, nullptr, &_image) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create image.");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(args.device, _image, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = _findMemoryType(
        args.physicalDevice, memoryRequirements.memoryTypeBits, args.propFlags);
    if (vkAllocateMemory(args.device, &allocInfo, nullptr, &_deviceMemory)) {
      throw std::runtime_error("Failed to allocate image memory.");
    }
    vkBindImageMemory(args.device, _image, _deviceMemory, 0);
    _isOwner = true;
  }

  uint32_t _findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
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
};

} /* namespace vkw */
#endif /* VKW_RESOURCE_IMAGE_HPP */