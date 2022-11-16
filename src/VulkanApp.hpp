#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace vkh {
/**
 * @brief
 *
 * @param pApplicationName
 * @return VkApplicationInfo
 */
VkApplicationInfo
makeApplicationInfo(const char *pApplicationName = "Vulkan Application") {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = pApplicationName;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;
  return appInfo;
}

/**
 * @brief flags not included
 *
 * @param appInfo
 * @param enabledLayerCount
 * @param ppEnabledLayerNames
 * @param pNext
 * @return VkInstanceCreateInfo
 */
VkInstanceCreateInfo
makeInstanceCreateInfo(const VkApplicationInfo *appInfo,
                       uint32_t enabledLayerCount = 0,
                       const char *const *ppEnabledLayerNames = nullptr,
                       const void *pNext = nullptr) {
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

/**
 * @brief
 *
 * @param pCreateInfo
 * @param pAllocator
 * @return VkInstance
 */
VkInstance createInstance(const VkInstanceCreateInfo *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator = nullptr) {
  VkInstance instance;
  if (vkCreateInstance(pCreateInfo, pAllocator, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create an instance.");
  }
  return instance;
}

/**
 * @brief Get physical devices with Vulkan support
 *
 * @param instance
 * @return std::vector<VkPhysicalDevice>
 */
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

/**
 * @brief
 *
 * @param width
 * @param height
 * @param title
 * @param monitor
 * @param share
 * @return GLFWwindow*
 */
GLFWwindow *createWindow(int width, int height,
                         const char *title = "Vulkan Application",
                         GLFWmonitor *monitor = nullptr,
                         GLFWwindow *share = nullptr) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  return glfwCreateWindow(width, height, title, monitor, share);
}

} /* namespace vkh */
