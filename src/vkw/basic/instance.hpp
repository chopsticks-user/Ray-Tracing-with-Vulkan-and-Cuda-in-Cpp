#ifndef VKW_INSTANCE_HPP
#define VKW_INSTANCE_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class Instance {
public:
  Instance() = default;
  Instance(VkInstanceCreateInfo *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _pAllocator{pAllocator} {
    vkCreateInstance(pCreateInfo, pAllocator, &_instance);
    if constexpr (enableValidationLayers) {
      _debugInfo = *(VkDebugUtilsMessengerCreateInfoEXT *)(pCreateInfo->pNext);
    }
    _isOwner = true;
  }
  Instance(const Instance &) = delete;
  Instance(Instance &&rhs) { _moveDataFrom(std::move(rhs)); }
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Instance() { _destroyVkData(); }

  const VkInstance &ref() const noexcept { return _instance; }

  const VkDebugUtilsMessengerCreateInfoEXT &debugInfo() const noexcept {
    return _debugInfo;
  }

protected:
  VkInstance _instance = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;
  VkDebugUtilsMessengerCreateInfoEXT _debugInfo = {};

  void _moveDataFrom(Instance &&rhs) {
    _instance = rhs._instance;
    _pAllocator = rhs._pAllocator;
    _debugInfo = std::move(rhs._debugInfo);
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyInstance(_instance, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Instance destructor" << '\n';
      }
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_INSTANCE_HPP */