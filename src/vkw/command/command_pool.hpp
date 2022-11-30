#ifndef VKW_COMMAND_POOL_HPP
#define VKW_COMMAND_POOL_HPP

#include "config.hpp"

namespace vkw {

class CommandPool {
public:
  CommandPool() = default;
  CommandPool(VkDevice device, uint32_t queueFamilyIndex,
              VkCommandPoolCreateFlags flags =
                  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
              const void *pNext = nullptr,
              const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _queueFamilyIndex{queueFamilyIndex}, _pAllocator{
                                                                  pAllocator} {
    VkCommandPoolCreateInfo cmdPoolInfo{};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = pNext;
    /* allows any command buffer allocated from a
    pool to be individually reset to the initial state; either by calling
    vkResetCommandBuffer, or via the implicit reset when calling
    vkBeginCommandBuffer. If this flag is not set on a pool, then
    vkResetCommandBuffer must not be called for any command buffer allocated
    from that pool. */
    cmdPoolInfo.flags = flags;
    /* All command buffers allocated from this command pool must be
    submitted on queues from the same queue family. */
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
    vkCreateCommandPool(device, &cmdPoolInfo, pAllocator, &_commandPool);
    _isOwner = true;
  }
  CommandPool(VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
              const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _queueFamilyIndex{pCreateInfo->queueFamilyIndex},
        _pAllocator{pAllocator} {
    vkCreateCommandPool(device, pCreateInfo, pAllocator, &_commandPool);
    _isOwner = true;
  }
  CommandPool(const CommandPool &) = delete;
  CommandPool(CommandPool &&rhs) { _moveDataFrom(std::move(rhs)); }
  CommandPool &operator=(const CommandPool &) = delete;
  CommandPool &operator=(CommandPool &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~CommandPool() { _destroyVkData(); }

  const VkCommandPool &ref() const noexcept { return _commandPool; }

  const VkDevice &device() const noexcept { return _device; }

  uint32_t queueFamilyIndex() const noexcept { return _queueFamilyIndex; }

  VkCommandBuffer
  allocateBuffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                 const void *pNext = nullptr) {
    VkCommandBufferAllocateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferInfo.pNext = pNext;
    bufferInfo.commandPool = _commandPool;
    bufferInfo.level = level;
    bufferInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(_device, &bufferInfo, &commandBuffer) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate command buffers.");
    }
    return commandBuffer;
  }

  std::vector<VkCommandBuffer>
  allocateBuffers(uint32_t count,
                  VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                  const void *pNext = nullptr) {
    VkCommandBufferAllocateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferInfo.pNext = pNext;
    bufferInfo.commandPool = _commandPool;
    bufferInfo.level = level;
    bufferInfo.commandBufferCount = count;
    std::vector<VkCommandBuffer> commandBuffers{static_cast<size_t>(count)};
    vkAllocateCommandBuffers(_device, &bufferInfo, commandBuffers.data());
    return commandBuffers;
  }

  void
  beginBuffer(VkCommandBuffer commandBuffer,
              VkCommandBufferUsageFlags flags =
                  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
              const VkCommandBufferInheritanceInfo *pInheritanceInfo = nullptr,
              const void *pNext = nullptr) {
    VkCommandBufferBeginInfo commandBufferInfo{};
    commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferInfo.pNext = pNext;
    commandBufferInfo.flags = flags;
    commandBufferInfo.pInheritanceInfo = pInheritanceInfo;
    if (vkBeginCommandBuffer(commandBuffer, &commandBufferInfo) != VK_SUCCESS) {
      throw std::runtime_error("Failed to begin command buffer.");
    }
  }

  void endBuffer(VkCommandBuffer commandBuffer) {
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("Failed to end command buffer.");
    }
  }

  void submitBuffer(VkQueue queue, const VkSubmitInfo *pSubmit,
                    VkFence fence = VK_NULL_HANDLE, bool wait = true) {
    vkQueueSubmit(queue, 1, pSubmit, fence);
    if (wait) {
      vkQueueWaitIdle(queue);
    }
  }

  void submitBuffer(VkQueue queue, VkCommandBuffer commandBuffer,
                    VkFence fence = VK_NULL_HANDLE, bool wait = true) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, fence);
    if (wait) {
      vkQueueWaitIdle(queue);
    }
  }

  void freeBuffer(VkCommandBuffer &commandBuffer) {
    vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
  }

  void freeBuffers(VkCommandBuffer &commandBuffer,
                   uint32_t commandBufferCount = 1) {
    vkFreeCommandBuffers(_device, _commandPool, commandBufferCount,
                         &commandBuffer);
  }

  void freeBuffers(std::vector<VkCommandBuffer> &commandBuffers) {
    vkFreeCommandBuffers(_device, _commandPool,
                         static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());
  }

protected:
  VkCommandPool _commandPool = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  uint32_t _queueFamilyIndex = 0;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(CommandPool &&rhs) {
    _commandPool = rhs._commandPool;
    _device = rhs._device;
    _queueFamilyIndex = rhs._queueFamilyIndex;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyCommandPool(_device, _commandPool, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "CommandPool destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_COMMAND_POOL_HPP */