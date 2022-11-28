#ifndef VKW_SYNC_SEMAPHORE_HPP
#define VKW_SYNC_SEMAPHORE_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class Semaphore {
public:
  Semaphore() = default;
  Semaphore(VkDevice device, VkSemaphoreCreateFlags flags = 0,
            const void *pNext = nullptr,
            const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.flags = flags;
    semaphoreInfo.pNext = pNext;
    vkCreateSemaphore(device, &semaphoreInfo, _pAllocator, &_semaphore);
    _isOwner = true;
  }
  Semaphore(const Semaphore &) = delete;
  Semaphore(Semaphore &&rhs) { _moveDataFrom(std::move(rhs)); }
  Semaphore &operator=(const Semaphore &) = delete;
  Semaphore &operator=(Semaphore &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Semaphore() { _destroyVkData(); }

  const VkSemaphore &ref() const noexcept { return _semaphore; }

private:
  VkSemaphore _semaphore;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Semaphore &&rhs) {
    _semaphore = rhs._semaphore;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroySemaphore(_device, _semaphore, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Semaphore destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_SYNC_SEMAPHORE_HPP */