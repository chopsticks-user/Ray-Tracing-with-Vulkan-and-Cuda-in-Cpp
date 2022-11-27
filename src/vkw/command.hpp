#ifndef VKW_COMMAND_HPP
#define VKW_COMMAND_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class CommandPool {
public:
  CommandPool() = default;
  CommandPool(VkDevice device, uint32_t queueFamilyIndex)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, queueFamilyIndex);
  }
  CommandPool(VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
              const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    _commandPool = vkh::createCommandPool(device, pCreateInfo, pAllocator);
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

  std::vector<VkCommandBuffer>
  allocateBuffers(uint32_t count,
                  VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                  const void *pNext = nullptr) {
    VkCommandBufferAllocateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferInfo.pNext = pNext;
    bufferInfo.commandPool = _commandPool;
    bufferInfo.level = level;
    bufferInfo.commandBufferCount = static_cast<uint32_t>(count);
    return vkh::allocateCommandBuffers(_device, &bufferInfo);
  }

private:
  VkCommandPool _commandPool;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(CommandPool &&rhs) {
    _commandPool = rhs._commandPool;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyCommandPool(_device, _commandPool, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "CommandPool destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device, uint32_t queueFamilyIndex) {
    VkCommandPoolCreateInfo cmdPoolInfo{};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = nullptr;
    /* allows any command buffer allocated from a
    pool to be individually reset to the initial state; either by calling
    vkResetCommandBuffer, or via the implicit reset when calling
    vkBeginCommandBuffer. If this flag is not set on a pool, then
    vkResetCommandBuffer must not be called for any command buffer allocated
    from that pool. */
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    /* All command buffers allocated from this command pool must be
    submitted on queues from the same queue family. */
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;

    _commandPool = vkh::createCommandPool(device, &cmdPoolInfo);
    _isOwner = true;
  }
};

// class CommandBuffers {
// public:
//   CommandPool() = default;
//   CommandPool(VkDevice device, uint32_t queueFamilyIndex)
//       : _device{device}, _pAllocator{nullptr} {
//     _customInitialize(device, queueFamilyIndex);
//   }
//   CommandPool(VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
//               const VkAllocationCallbacks *pAllocator = nullptr)
//       : _device{device}, _pAllocator{pAllocator} {
//     _commandPool = vkh::createCommandPool(device, pCreateInfo, pAllocator);
//     _isOwner = true;
//   }
//   CommandPool(const CommandPool &) = delete;
//   CommandPool(CommandPool &&rhs) { _moveDataFrom(std::move(rhs)); }
//   CommandPool &operator=(const CommandPool &) = delete;
//   CommandPool &operator=(CommandPool &&rhs) {
//     _moveDataFrom(std::move(rhs));
//     return *this;
//   }
//   ~CommandPool() { _destroyVkData(); }

//   const VkCommandPool &ref() const noexcept { return _commandPool; }

// private:
//   VkCommandPool _commandPool;
//   VkDevice _device;
//   const VkAllocationCallbacks *_pAllocator;
//   bool _isOwner = false;

//   void _moveDataFrom(CommandPool &&rhs) {

//     if (rhs._isOwner) {
//       _isOwner = true;
//       rhs._isOwner = false;
//     }
//   }

//   void _destroyVkData() {
//     if (_isOwner) {
//       vkh::destroyCommandPool(_device, _commandPool, _pAllocator);
//       _isOwner = false;
//       if constexpr (enableValidationLayers) {
//         std::cout << "CommandBuffers destructor" << '\n';
//       }
//     }
//   }
// };

} /* namespace vkw */
#endif /* VKW_COMMAND_HPP */