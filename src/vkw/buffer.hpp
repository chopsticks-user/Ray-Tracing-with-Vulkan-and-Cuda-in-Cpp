#ifndef VKW_BUFFER_HPP
#define VKW_BUFFER_HPP

#include "config.hpp"

#include <cstring>
#include <vkh.hpp>

namespace vkw {

class Buffer {
public:
  Buffer() = default;
  Buffer(VkDevice device, const VkBufferCreateInfo *pCreateInfo,
         const VkMemoryAllocateInfo *pAllocInfo, VkDeviceSize memoryOffset = 0,
         const VkAllocationCallbacks *pBufferAllocator = nullptr,
         const VkAllocationCallbacks *pMemoryAllocator = nullptr)
      : _device{device}, _memoryOffset{memoryOffset},
        _pBufferAllocator{pBufferAllocator}, _pMemoryAllocator{
                                                 pMemoryAllocator} {
    if (vkCreateBuffer(device, pCreateInfo, pBufferAllocator, &_buffer) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed creating buffer.");
    }
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, _buffer, &memoryRequirements);
    if (vkAllocateMemory(device, pAllocInfo, nullptr, &_deviceMemory) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate buffer memory.");
    }
    vkBindBufferMemory(device, _buffer, _deviceMemory, memoryOffset);
    _isOwner = true;
  }
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&rhs) { _moveDataFrom(std::move(rhs)); }
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Buffer() { _destroyVkData(); }

  const VkBuffer &ref() const noexcept { return _buffer; }

  const VkDeviceSize &offset() const noexcept { return _memoryOffset; }

  template <typename DataType>
  void getHostData(DataType *pHostData, VkDeviceSize size,
                   VkMemoryMapFlags flags = 0) {
    DataType *pTempData;
    vkMapMemory(_device, _buffer, _memoryOffset, size, flags, &pTempData);
    std::memcpy(pTempData, pHostData, size);
    vkUnmapMemory(_device, _deviceMemory);
  }

private:
  VkBuffer _buffer;
  VkDeviceMemory _deviceMemory;
  VkDevice _device;
  VkDeviceSize _memoryOffset = 0;
  const VkAllocationCallbacks *_pBufferAllocator = nullptr;
  const VkAllocationCallbacks *_pMemoryAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Buffer &&rhs) {
    _buffer = rhs._buffer;
    _deviceMemory = rhs._deviceMemory;
    _device = rhs._device;
    _memoryOffset = rhs._memoryOffset;
    _pBufferAllocator = rhs._pBufferAllocator;
    _pMemoryAllocator = rhs._pMemoryAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyBuffer(_device, _buffer, _pBufferAllocator);
      vkFreeMemory(_device, _deviceMemory, _pMemoryAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Buffer destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_BUFFER_HPP */