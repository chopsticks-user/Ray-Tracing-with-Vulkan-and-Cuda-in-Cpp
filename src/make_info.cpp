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
    const VkApplicationInfo *appInfo, uint32_t enabledLayerCount,
    const char *const *ppEnabledLayerNames, const void *pNext) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pNext = pNext;
  createInfo.pApplicationInfo = appInfo;
  createInfo.enabledLayerCount = enabledLayerCount;
  createInfo.ppEnabledLayerNames = ppEnabledLayerNames;

  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;

  uint32_t glfwExtensionCount;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  return createInfo;
}

} // namespace vkh