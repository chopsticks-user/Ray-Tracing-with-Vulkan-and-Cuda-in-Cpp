#ifndef VKW_DEBUG_HPP
#define VKW_DEBUG_HPP

#include "config.hpp"

#include <vkh.hpp>

#include <string>

namespace vkw {

inline VkResult createDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func == nullptr) {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
  return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

inline VkDebugUtilsMessengerEXT
createDebugMessenger(VkInstance instance,
                     const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                     const VkAllocationCallbacks *pAllocator) {
  VkDebugUtilsMessengerEXT debugMessenger;
  if (createDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator,
                                   &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug message.");
  }
  return debugMessenger;
}

inline void destroyDebugMessenger(VkInstance instance,
                                  VkDebugUtilsMessengerEXT debugMessenger,
                                  const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

class DebugMessenger {
public:
  static constexpr std::string_view validationLayerName =
      "VK_LAYER_KHRONOS_validation";

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
          createDebugMessenger(_instance, pCreateInfo, pAllocator);
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
      destroyDebugMessenger(_instance, _debugMessenger, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "DebugMessenger destructor" << '\n';
      }
    }
  }

  bool _checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers{layerCount};
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto &layerProperties : availableLayers) {
      if (validationLayerName == layerProperties.layerName) {
        return true;
      }
    }
    return false;
  }
};

} /* namespace vkw */
#endif /* VKW_DEBUG_HPP */