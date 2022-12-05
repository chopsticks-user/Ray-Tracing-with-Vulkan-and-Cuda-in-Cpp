#include "instance.hpp"

#include <iostream>

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

bool validationLayerSupported() {
  auto supportedLayers = vk::enumerateInstanceLayerProperties();
  for (const auto &supportedLayer : supportedLayers) {
    if (supportedLayer.layerName == validationLayerName) {
      return true;
    }
  }
  return false;
}

vk::raii::Instance makeInstance(const vk::raii::Context &context) {
  vk::ApplicationInfo applicationInfo{
      .pApplicationName{"Vulkan Application"},
      .applicationVersion{VK_MAKE_VERSION(1, 0, 0)},
      .apiVersion{VK_API_VERSION_1_0},
  };

  // Required extensions
  uint32_t requiredExtensionCount = 0;
  auto ppRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char *> requiredExtensions{requiredExtensionCount};
  for (size_t i = 0; i < requiredExtensionCount; ++i) {
    requiredExtensions[i] = ppRequiredExtensions[i];
  }

  // Requested layers
  std::vector<const char *> requestedLayers = {
      mangohudLayerName.data(),
  };

  // Enable validation layers if the application is running in debug mode
  vk::DebugUtilsMessengerCreateInfoEXT *pDebugInfo = nullptr;
  vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
  if constexpr (debugMode) {
    if (validationLayerSupported()) {
      requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      requestedLayers.push_back(validationLayerName.data());

      debugInfo = makeDebugMessengerInfo();
      pDebugInfo = &debugInfo;
    }
  }

  vk::InstanceCreateInfo instanceInfo{
      .pNext{pDebugInfo},
      .pApplicationInfo{&applicationInfo},
      .enabledLayerCount{static_cast<uint32_t>(requestedLayers.size())},
      .ppEnabledLayerNames{requestedLayers.data()},
      .enabledExtensionCount{static_cast<uint32_t>(requiredExtensions.size())},
      .ppEnabledExtensionNames{requiredExtensions.data()},
  };
  return {context, instanceInfo};
}

[[nodiscard]] vk::DebugUtilsMessengerCreateInfoEXT makeDebugMessengerInfo() {
  return {
      .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
      .messageType =
          vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
      .pfnUserCallback = debugCallback,
  };
}

vk::raii::DebugUtilsMessengerEXT
makeDebugMessenger(const vk::raii::Instance &instance) {
  if constexpr (debugMode) {
    return {instance, makeDebugMessengerInfo()};
  }
  return {VK_NULL_HANDLE};
}

vk::raii::SurfaceKHR makeSurface(const vk::raii::Instance &instance,
                                 const Window &window) {
  VkSurfaceKHR surface;
  glfwCreateWindowSurface(static_cast<VkInstance>(*instance), *window, nullptr,
                          &surface);
  return {instance, surface};
}

} /* namespace rtvc */