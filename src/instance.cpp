#include "instance.hpp"

namespace rtvc {

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData) {
  if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    std::cerr << '\n' << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

void Instance::_initialize() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  uint32_t requiredExtensionCount;
  const char **ppRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char *> requiredExtensions{requiredExtensionCount};
  for (size_t i = 0; i < requiredExtensionCount; ++i) {
    requiredExtensions[i] = ppRequiredExtensions[i];
  }

  std::vector<const char *> layers = {"VK_LAYER_MANGOHUD_overlay"};

  VkInstanceCreateInfo instanceInfo{};

  if constexpr (vkw::enableValidationLayers) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    layers.emplace_back("VK_LAYER_KHRONOS_validation");

    VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfo.pfnUserCallback = debugCallback;
    debugInfo.pUserData = nullptr;

    instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugInfo;

    _debugInfo = debugInfo;
  }

  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
  instanceInfo.ppEnabledLayerNames = layers.data();
  instanceInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensions.size());
  instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
  vkCreateInstance(&instanceInfo, nullptr, &_instance);
  _isOwner = true;
}

} /* namespace rtvc */