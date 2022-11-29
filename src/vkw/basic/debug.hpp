#ifndef VKW_DEBUG_HPP
#define VKW_DEBUG_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class DebugMessenger {
public:
  DebugMessenger() = default;
  DebugMessenger(VkInstance instance,
                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                 const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _pAllocator{pAllocator} {
    if constexpr (vkw::enableValidationLayers) {
      if (vkh::checkValidationLayerSupport() == false) {
        throw std::runtime_error("Validation layers are not supported.");
      }
      _debugMessenger =
          vkh::createDebugMessenger(_instance, pCreateInfo, pAllocator);
      _isOwner = true;
    }
  }
  DebugMessenger(const DebugMessenger &) = delete;
  DebugMessenger(DebugMessenger &&rhs) { _moveDataFrom(std::move(rhs)); }
  DebugMessenger &operator=(const DebugMessenger &) = delete;
  DebugMessenger &operator=(DebugMessenger &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~DebugMessenger() { _destroyVkData(); }

  const VkDebugUtilsMessengerEXT &ref() const noexcept {
    return _debugMessenger;
  }

protected:
  VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
  VkInstance _instance = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(DebugMessenger &&rhs) {
    if constexpr (vkw::enableValidationLayers) {
      _debugMessenger = rhs._debugMessenger;
      _instance = rhs._instance;
      _pAllocator = rhs._pAllocator;
      _isOwner = true;
    }
    rhs._isOwner = false;
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyDebugMessenger(_instance, _debugMessenger, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "DebugMessenger destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_DEBUG_HPP */