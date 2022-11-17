#include "get_list.hpp"

namespace vkh {

std::vector<VkExtensionProperties>
getAvailableInstanceExtensionList(const char *pLayerName) {
  uint32_t propertyCount;
  if (vkEnumerateInstanceExtensionProperties(pLayerName, &propertyCount,
                                             nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get instance extension list.");
  }
  std::vector<VkExtensionProperties> instanceExtensions{propertyCount};
  if (vkEnumerateInstanceExtensionProperties(pLayerName, &propertyCount,
                                             instanceExtensions.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to get instance extension list.");
  }
  return instanceExtensions;
}

std::vector<VkExtensionProperties>
getAvailableDeviceExtensionList(VkPhysicalDevice physicalDevice,
                                const char *pLayerName) {
  uint32_t propertyCount;
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, pLayerName, &propertyCount, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }
  std::vector<VkExtensionProperties> deviceExtensions{propertyCount};
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, pLayerName, &propertyCount,
          deviceExtensions.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }
  return deviceExtensions;
}

std::vector<const char *>
getRequiredInstanceExtensionNameList(bool enableValidationLayers) {
  uint32_t requiredExtensionCount;
  const char **ppRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char *> requiredInstanceExtensions{requiredExtensionCount};
  for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
    requiredInstanceExtensions[i] = ppRequiredExtensions[i];
  }
  if (enableValidationLayers) {
    requiredInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  return requiredInstanceExtensions;
}

std::vector<VkLayerProperties> getAvailableInstanceLayerList() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers{layerCount};
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
  return availableLayers;
}

std::vector<VkPhysicalDevice> getPhysicalDeviceList(VkInstance instance) {
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
  if (physicalDeviceCount == 0) {
    throw std::runtime_error(
        "Failed to find any physical device with Vulkan support.");
  }
  std::vector<VkPhysicalDevice> physicalDevices{physicalDeviceCount};
  if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
                                 physicalDevices.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get physical devices list.");
  };
  return physicalDevices;
}

VkPhysicalDeviceProperties
getPhysicalDevicePropertyList(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);
  return properties;
}

VkPhysicalDeviceProperties2
getPhysicalDevicePropertyList(VkPhysicalDevice physicalDevice,
                              void *pNextProperty) {
  VkPhysicalDeviceProperties2 properties;
  properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
  properties.pNext = pNextProperty;
  vkGetPhysicalDeviceProperties2(physicalDevice, &properties);
  return properties;
}

std::vector<VkQueueFamilyProperties>
getPhysicalDeviceQueuePropertyList(VkPhysicalDevice physicalDevice) {
  uint32_t queueFamilyPropertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                           &queueFamilyPropertyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilyProperties{
      queueFamilyPropertyCount};
  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());
  return queueFamilyProperties;
}

} /* namespace vkh */