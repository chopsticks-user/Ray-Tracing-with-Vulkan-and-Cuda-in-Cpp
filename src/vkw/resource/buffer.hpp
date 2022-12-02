#ifndef VKW_RESOURCE_BUFFER_HPP
#define VKW_RESOURCE_BUFFER_HPP

#include "config.hpp"

#include "command/command_pool.hpp"

#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace vkw {

class Buffer {
public:
  Buffer() = default;
  Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,
         VkBufferUsageFlags usage, VkMemoryPropertyFlags propertyFlags)
      : _device{device} {
    _customInitialize(device, physicalDevice, size, usage, propertyFlags);
  }
  Buffer(VkDevice device, const VkBufferCreateInfo *pCreateInfo,
         const VkMemoryAllocateInfo *pAllocInfo,
         const VkAllocationCallbacks *pBufferAllocator = nullptr,
         const VkAllocationCallbacks *pMemoryAllocator = nullptr)
      : _device{device}, _pBufferAllocator{pBufferAllocator},
        _pMemoryAllocator{pMemoryAllocator} {
    _createAllocatedBuffer(pCreateInfo, pAllocInfo);
    vkBindBufferMemory(device, _buffer, _deviceMemory, 0);
    _isOwner = true;
  }
  Buffer(VkDevice device, VkDeviceSize memoryOffset,
         const VkBufferCreateInfo *pCreateInfo,
         const VkMemoryAllocateInfo *pAllocInfo,
         const VkAllocationCallbacks *pBufferAllocator = nullptr,
         const VkAllocationCallbacks *pMemoryAllocator = nullptr)
      : _device{device}, _memoryOffset{memoryOffset},
        _pBufferAllocator{pBufferAllocator}, _pMemoryAllocator{
                                                 pMemoryAllocator} {
    _createAllocatedBuffer(pCreateInfo, pAllocInfo);
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
  virtual ~Buffer() { _destroyVkData(); }

  const VkBuffer &ref() const noexcept { return _buffer; }

  const VkDeviceMemory &memory() const noexcept { return _deviceMemory; }

  const VkDeviceSize &offset() const noexcept { return _memoryOffset; }

  template <typename DataType>
  void copyHostData(DataType *pHostData, VkDeviceSize size,
                    VkMemoryMapFlags flags = 0) {
    void *data;
    vkMapMemory(_device, _deviceMemory, _memoryOffset, size, flags, &data);
    std::memcpy(data, pHostData, size);
    vkUnmapMemory(_device, _deviceMemory);
  }

  void copyDeviceData(vkw::CommandPool &commandPool, VkQueue queue,
                      const vkw::Buffer &src, VkDeviceSize size) {
    auto commandBuffer =
        commandPool.allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    commandPool.beginBuffer(commandBuffer);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = src._memoryOffset;
    copyRegion.dstOffset = _memoryOffset;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, src.ref(), _buffer, 1, &copyRegion);

    commandPool.endBuffer(commandBuffer);

    /* Submit and wait on this transfer to complete before cleaning up
    the command buffer */
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    commandPool.submitBuffer(queue, &submitInfo);
    commandPool.freeBuffer(commandBuffer);
  }

protected:
  VkBuffer _buffer;
  VkDevice _device;
  VkDeviceMemory _deviceMemory;
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
    }
  }

  void _createAllocatedBuffer(const VkBufferCreateInfo *pCreateInfo,
                              const VkMemoryAllocateInfo *pAllocInfo) {
    if (vkCreateBuffer(_device, pCreateInfo, _pBufferAllocator, &_buffer) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed creating buffer.");
    }
    if (vkAllocateMemory(_device, pAllocInfo, _pMemoryAllocator,
                         &_deviceMemory) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate buffer memory.");
    }
  }

  CUSTOM virtual void _customInitialize(VkDevice device,
                                        VkPhysicalDevice physicalDevice,
                                        VkDeviceSize size,
                                        VkBufferUsageFlags usage,
                                        VkMemoryPropertyFlags propertyFlags) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
      throw std::runtime_error("Failed creating buffer.");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, _buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = _findMemoryType(
        physicalDevice, memoryRequirements.memoryTypeBits, propertyFlags);
    if (vkAllocateMemory(device, &allocInfo, nullptr, &_deviceMemory) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate buffer memory.");
    }
    vkBindBufferMemory(device, _buffer, _deviceMemory, 0);
    _isOwner = true;
  }

private:
  CUSTOM uint32_t _findMemoryType(VkPhysicalDevice physicalDevice,
                                  uint32_t typeFilter,
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
#endif /* VKW_RESOURCE_BUFFER_HPP */