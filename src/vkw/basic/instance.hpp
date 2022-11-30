#ifndef VKW_INSTANCE_HPP
#define VKW_INSTANCE_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class Instance {
public:
  Instance() {
    _instance = _customInitialize(_debugInfo);
    _isOwner = true;
  }
  Instance(VkInstanceCreateInfo *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _pAllocator{pAllocator} {
    _instance = vkh::createInstance(pCreateInfo, pAllocator);
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
      vkh::destroyInstance(_instance, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Instance destructor" << '\n';
      }
    }
  }

private:
  CUSTOM static VkInstance
  _customInitialize(VkDebugUtilsMessengerCreateInfoEXT &debInfo) {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    std::vector<const char *> requiredExtensions{
        vkh::getRequiredInstanceExtensionNameList()};
    std::vector<const char *> layers = {"VK_LAYER_MANGOHUD_overlay"};
    ;
    VkInstanceCreateInfo instanceInfo{};

    if constexpr (vkw::enableValidationLayers) {
      requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      layers.push_back("VK_LAYER_KHRONOS_validation");
      VkDebugUtilsMessengerCreateInfoEXT debugInfo;
      populateDebugMessengerCreateInfo(debugInfo);
      instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugInfo;
      debInfo = debugInfo;
    }

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instanceInfo.ppEnabledLayerNames = layers.data();
    instanceInfo.enabledExtensionCount =
        static_cast<uint32_t>(requiredExtensions.size());
    instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
    return vkh::createInstance(&instanceInfo);
  }

  CUSTOM static void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = vkh::debugCallback;
    createInfo.pUserData = nullptr;
  }
};

} /* namespace vkw */
#endif /* VKW_INSTANCE_HPP */