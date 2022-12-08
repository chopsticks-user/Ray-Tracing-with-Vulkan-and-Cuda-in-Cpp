#include "logical.hpp"

#include "renderer/basic/instance/instance.hpp"
#include "renderer/basic/surface/surface.hpp"
// #include "renderer/devices/physical/physical.hpp"

#include <map>
#include <optional>

namespace neko {

LogicalDevice::LogicalDevice(const Instance &crInstance,
                             const Surface &crSurface) {
  VkPhysicalDevice selectedPhysicalDevice =
      selectPhysicalDevice(getPhysicalDevices(*crInstance));

  uint32_t selectedQueueFamilyIndex =
      selectUniversalQueueFamily(selectedPhysicalDevice, *crSurface);
  uint32_t selectedQueueIndex = 0;

  float queuePriority = 1.0f;

  /* Populate the selected queue family's creation info */
  VkDeviceQueueCreateInfo queueInfo{};
  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueInfo.pNext = nullptr;
  queueInfo.queueFamilyIndex = selectedQueueFamilyIndex;
  queueInfo.queueCount = 1;
  queueInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures{};
  //   deviceFeatures.samplerAnisotropy = VK_TRUE;
  //   deviceFeatures.sampleRateShading = VK_TRUE;

  /* Populate the logical device's creation info */
  VkDeviceCreateInfo deviceInfo{};
  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queueInfo;
  deviceInfo.pEnabledFeatures = &deviceFeatures;

  /* Create a logical device */
  if (vkCreateDevice(selectedPhysicalDevice, &deviceInfo, nullptr,
                     &mLogicalDevice) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device.");
  }

  /* Get handles */
  mPhysicalDevice = selectedPhysicalDevice;

  VkQueue queueHandle;
  vkGetDeviceQueue(mLogicalDevice, selectedQueueFamilyIndex, selectedQueueIndex,
                   &queueHandle);
  mQueue = {selectedQueueFamilyIndex, selectedQueueIndex, queueHandle};
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(mLogicalDevice, nullptr); }

uint32_t
LogicalDevice::selectUniversalQueueFamily(VkPhysicalDevice physicalDevice,
                                          VkSurfaceKHR surface) {
  uint32_t queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                           nullptr);
  if (queueFamilyCount == 0) {
    throw std::runtime_error("No queue family available.");
  }

  std::vector<VkQueueFamilyProperties> queueFamilyProperties{queueFamilyCount};
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                           queueFamilyProperties.data());

  /* Find a queue that is capable of handling graphics, computing, and
  supporting presentation to the surface */
  /* any queue family with {VK_QUEUE_GRAPHICS_BIT} or {VK_QUEUE_COMPUTE_BIT}
  capabilities already implicitly support {VK_QUEUE_TRANSFER_BIT} operations */
  uint32_t iQueueFamily = 0;
  for (const auto &queueFamily : queueFamilyProperties) {
    if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
        (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, iQueueFamily,
                                           surface, &presentSupport);
      if (presentSupport) {
        return iQueueFamily;
      }
      ++iQueueFamily;
    }
  }
  throw std::runtime_error("Failed to select a queue family.");
}

VkPhysicalDevice LogicalDevice::selectPhysicalDevice(
    std::vector<VkPhysicalDevice> &&rrPhysicalDevices) {
  if (rrPhysicalDevices.size() == 1) {
    return rrPhysicalDevices[0];
  }
  for (size_t iDevice = 0; iDevice < rrPhysicalDevices.size(); ++iDevice) {
    if (checkRequirements(rrPhysicalDevices[iDevice])) {
      return rrPhysicalDevices[iDevice];
    }
  }
  throw std::runtime_error("No suitable physical device found.");
}

bool LogicalDevice::checkRequirements(VkPhysicalDevice physicalDevice) {
  return checkProperties(physicalDevice) && checkExtensions(physicalDevice) &&
         checkFeatures(physicalDevice);
}

bool LogicalDevice::checkProperties(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);
  return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

bool LogicalDevice::checkExtensions(VkPhysicalDevice physicalDevice) {
  const std::vector<const char *> requiredExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  uint32_t propertyCount;
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &propertyCount, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }
  std::vector<VkExtensionProperties> availableExtensions{propertyCount};
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &propertyCount,
          availableExtensions.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }

  /* Check if all required extension are supported */
  std::map<std::string, uint32_t> helper;
  for (const auto &availableExtension : availableExtensions) {
    helper[availableExtension.extensionName]++;
  }
  for (const auto &requiredExtension : requiredExtensions) {
    helper[requiredExtension]++;
    if (helper[requiredExtension] < 2) {
      return false;
    }
  }
  return true;
}

bool LogicalDevice::checkFeatures(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
  return supportedFeatures.samplerAnisotropy;
}

std::vector<VkPhysicalDevice>
LogicalDevice::getPhysicalDevices(VkInstance instance) {
  uint32_t physicalDeviceCount;
  VkResult result;
  if (result =
          vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
      result != VK_SUCCESS && result != VK_INCOMPLETE) {
    throw std::runtime_error("Failed to enumerate physical devices.");
  }
  if (physicalDeviceCount == 0) {
    throw std::runtime_error("Failed to find any physical device.");
  }
  std::vector<VkPhysicalDevice> physicalDevices{physicalDeviceCount};
  if (result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
                                          physicalDevices.data());
      result != VK_SUCCESS && result != VK_INCOMPLETE) {
    throw std::runtime_error("Failed to enumerate physical devices.");
  }
  return physicalDevices;
}

} /* namespace neko */