#ifndef VKW_SYNC_FENCE_HPP
#define VKW_SYNC_FENCE_HPP

#include "config.hpp"

namespace vkw {

class Fence {
public:
  Fence() = default;
  Fence(VkDevice device, VkFenceCreateFlags flags = 0,
        const void *pNext = nullptr,
        const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = flags;
    fenceInfo.pNext = pNext;
    vkCreateFence(device, &fenceInfo, _pAllocator, &_fence);
    _isOwner = true;
  }
  Fence(const Fence &) = delete;
  Fence(Fence &&rhs) { _moveDataFrom(std::move(rhs)); }
  Fence &operator=(const Fence &) = delete;
  Fence &operator=(Fence &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Fence() { _destroyVkData(); }

  const VkFence &ref() const noexcept { return _fence; }

private:
  VkFence _fence;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Fence &&rhs) {
    _fence = rhs._fence;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyFence(_device, _fence, _pAllocator);
      _isOwner = false;
    }
  }
};

} /* namespace vkw */
#endif /* VKW_SYNC_FENCE_HPP */