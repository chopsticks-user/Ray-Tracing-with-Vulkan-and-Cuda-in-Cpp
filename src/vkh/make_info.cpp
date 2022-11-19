#include "make_info.hpp"

namespace vkh {
VkApplicationInfo makeApplicationInfo(const char *pApplicationName) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = pApplicationName;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;
  return appInfo;
}

VkInstanceCreateInfo makeInstanceCreateInfo(
    const VkApplicationInfo *appInfo, uint32_t enabledExtensionCount,
    const char *const *ppEnabledExtensionNames, uint32_t enabledLayerCount,
    const char *const *ppEnabledLayerNames, const void *pNext) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pNext = pNext;
  createInfo.pApplicationInfo = appInfo;
  createInfo.enabledLayerCount = enabledLayerCount;
  createInfo.ppEnabledLayerNames = ppEnabledLayerNames;
  createInfo.enabledExtensionCount = enabledExtensionCount;
  createInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
  return createInfo;
}

VkDeviceCreateInfo makeDeviceCreateInfo(
    uint32_t queueCreateInfoCount,
    const VkDeviceQueueCreateInfo *pQueueCreateInfos,
    uint32_t enabledExtensionCount, const char *const *ppEnabledExtensionNames,
    const VkPhysicalDeviceFeatures *pEnabledFeatures,
    uint32_t enabledLayerCount, const char *const *ppEnabledLayerNames,
    const void *pNext) {
  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pNext = pNext;
  deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
  deviceCreateInfo.pQueueCreateInfos = pQueueCreateInfos;
  deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
  deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
  deviceCreateInfo.pEnabledFeatures = pEnabledFeatures;
  deviceCreateInfo.enabledLayerCount = enabledLayerCount;
  deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
  return deviceCreateInfo;
}

VkDeviceQueueCreateInfo makeDeviceQueueCreateInfo(uint32_t queueFamilyIndex,
                                                  uint32_t queueCount,
                                                  const float *pQueuePriorities,
                                                  const void *pNext) {
  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.pNext = pNext;
  queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
  queueCreateInfo.queueCount = queueCount;
  queueCreateInfo.pQueuePriorities = pQueuePriorities;
  return queueCreateInfo;
}

} /* namespace vkh */