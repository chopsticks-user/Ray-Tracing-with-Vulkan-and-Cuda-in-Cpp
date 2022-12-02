#ifndef VKW_DEVICE_HPP
#define VKW_DEVICE_HPP

#include "config.hpp"

namespace vkw {

class Device {
public:
  Device() = default;
  Device(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo,
         const VkAllocationCallbacks *pAllocator = nullptr)
      : _physicalDevice{physicalDevice}, _pAllocator{pAllocator} {
    vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, &_device);
    if (pCreateInfo->queueCreateInfoCount != 1) {
      throw std::runtime_error("pCreateInfo->queueCreateInfoCount != 1");
    }
    _queue.familyIndex = pCreateInfo->pQueueCreateInfos->queueFamilyIndex;
    vkGetDeviceQueue(_device, _queue.familyIndex, 0, &_queue.ref);
    _isOwner = true;
  };
  Device(const Device &) = delete;
  Device(Device &&rhs) { _moveDataFrom(std::move(rhs)); }
  Device &operator=(const Device &) = delete;
  Device &operator=(Device &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Device() { _destroyVkData(); }

  const VkDevice &ref() const noexcept { return _device; }

  const VkPhysicalDevice &physical() const noexcept { return _physicalDevice; }

  VkQueue &queue() noexcept { return _queue.ref; }

  template <typename ReturnType = uint32_t> auto familyIndex() const noexcept {
    return static_cast<ReturnType>(_queue.familyIndex);
  }

  void waitIdle() { vkDeviceWaitIdle(_device); }

protected:
  VkDevice _device = VK_NULL_HANDLE;
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  MultiPurposeQueue _queue = {};
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Device &&rhs) {
    _destroyVkData();
    _device = rhs._device;
    _physicalDevice = rhs._physicalDevice;
    _queue = std::move(rhs._queue);
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyDevice(_device, _pAllocator);
      _isOwner = false;
    }
  }
};

} /* namespace vkw */
#endif /* VKW_DEVICE_HPP */