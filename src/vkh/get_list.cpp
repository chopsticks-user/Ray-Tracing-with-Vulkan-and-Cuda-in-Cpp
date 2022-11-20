#include "get_list.hpp"

namespace vkh {

std::vector<VkLayerProperties> getAvailableInstanceLayerPropertyList() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers{layerCount};
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
  return availableLayers;
}

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

std::vector<const char *> getRequiredInstanceExtensionNameList() {
  uint32_t requiredExtensionCount;
  const char **ppRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char *> requiredInstanceExtensions{requiredExtensionCount};
  for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
    requiredInstanceExtensions[i] = ppRequiredExtensions[i];
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
getPhysicalDeviceQueueFamilyPropertyList(VkPhysicalDevice physicalDevice) {
  uint32_t queueFamilyPropertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                           &queueFamilyPropertyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilyProperties{
      queueFamilyPropertyCount};
  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());
  return queueFamilyProperties;
}

std::vector<std::optional<VkQueueFamilyProperties>>
getGraphicsQueueFamilyPropertyList(VkPhysicalDevice physicalDevice) {
  auto queueFamilyProps =
      getPhysicalDeviceQueueFamilyPropertyList(physicalDevice);
  std::vector<std::optional<VkQueueFamilyProperties>> graphicsQueueFamilies{
      queueFamilyProps.size()};
  std::size_t index = 0;
  for (const auto &queueFamilyProp : queueFamilyProps) {
    if (queueFamilyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphicsQueueFamilies[index] = queueFamilyProp;
    }
  }
  return graphicsQueueFamilies;
}

std::vector<std::optional<VkQueueFamilyProperties>>
getComputeQueueFamilyPropertyList(VkPhysicalDevice physicalDevice) {
  auto queueFamilyProps =
      getPhysicalDeviceQueueFamilyPropertyList(physicalDevice);
  std::vector<std::optional<VkQueueFamilyProperties>> computeQueueFamilies{
      queueFamilyProps.size()};
  std::size_t index = 0;
  for (const auto &queueFamilyProp : queueFamilyProps) {
    if (queueFamilyProp.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      computeQueueFamilies[index] = queueFamilyProp;
    }
    ++index;
  }
  return computeQueueFamilies;
}

std::vector<std::optional<VkQueueFamilyProperties>>
getPresentQueueFamilyPropertyList(VkPhysicalDevice physicalDevice,
                                  VkSurfaceKHR surface) {
  auto queueFamilyProps =
      getPhysicalDeviceQueueFamilyPropertyList(physicalDevice);
  std::vector<std::optional<VkQueueFamilyProperties>> presentQueueFamilies{
      queueFamilyProps.size()};
  uint32_t index = 0;
  for (const auto &queueFamilyProp : queueFamilyProps) {
    VkBool32 presentSupported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface,
                                         &presentSupported);
    if (presentSupported) {
      presentQueueFamilies[index] = queueFamilyProp;
    }
    ++index;
  }
  return presentQueueFamilies;
}

VkSurfaceCapabilitiesKHR
getPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physicalDevice,
                                     VkSurfaceKHR surface) {
  VkSurfaceCapabilitiesKHR surfaceCapabilities{};
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
          physicalDevice, surface, &surfaceCapabilities) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get surface capabilities.");
  }
  return surfaceCapabilities;
}

std::vector<VkSurfaceFormatKHR>
getPhysicalDeviceSurfaceFormatList(VkPhysicalDevice physicalDevice,
                                   VkSurfaceKHR surface) {
  uint32_t surfaceFormatCount;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,
                                           &surfaceFormatCount,
                                           nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get surface format list.");
  }
  std::vector<VkSurfaceFormatKHR> surfaceFormats{surfaceFormatCount};
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(
          physicalDevice, surface, &surfaceFormatCount,
          surfaceFormats.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get surface format list.");
  }
  return surfaceFormats;
}
std::vector<VkPresentModeKHR>
getPhysicalDeviceSurfacePresentModeList(VkPhysicalDevice physicalDevice,
                                        VkSurfaceKHR surface) {
  uint32_t presentModeCount;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                                &presentModeCount, nullptr)) {
    throw std::runtime_error("Failed to get surface present mode list.");
  }
  std::vector<VkPresentModeKHR> presentModes{presentModeCount};
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(
          physicalDevice, surface, &presentModeCount, presentModes.data())) {
    throw std::runtime_error("Failed to get surface present mode list.");
  }
  return presentModes;
}

std::vector<VkImage> getSwapchainImages(VkDevice device,
                                        VkSwapchainKHR swapchain) {
  uint32_t swapchainImageCount;
  if (vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount,
                              nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get swapchain images");
  }
  std::vector<VkImage> swapchainImages{swapchainImageCount};
  if (vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount,
                              swapchainImages.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get swapchain images");
  }
  return swapchainImages;
}

} /* namespace vkh */