#include "validation_layers.hpp"

namespace vkh {

bool checkValidationLayerSupport() {
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

VkResult createDebugUtilsMessengerEXT(
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

VkDebugUtilsMessengerEXT
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

void destroyDebugMessenger(VkInstance instance,
                           VkDebugUtilsMessengerEXT debugMessenger,
                           const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

} /* namespace vkh */