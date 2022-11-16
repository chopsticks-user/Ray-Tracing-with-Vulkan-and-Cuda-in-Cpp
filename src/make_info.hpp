#ifndef VKH_MAKE_INFO_HPP
#define VKH_MAKE_INFO_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "get_list.hpp"

namespace vkh {
/**
 * @brief
 *
 * @param pApplicationName
 * @return VkApplicationInfo
 */
VkApplicationInfo
makeApplicationInfo(const char *pApplicationName = "Vulkan Application");

/**
 * @brief
 *
 * @param appInfo
 * @param enabledLayerCount
 * @param ppEnabledLayerNames
 * @param pNext
 * @return VkInstanceCreateInfo
 */
VkInstanceCreateInfo
makeInstanceCreateInfo(const VkApplicationInfo *appInfo,
                       uint32_t enabledExtensionCount = 0,
                       const char *const *ppEnabledExtensionNames = nullptr,
                       uint32_t enabledLayerCount = 0,
                       const char *const *ppEnabledLayerNames = nullptr,
                       const void *pNext = nullptr);

/**
 * @brief
 *
 * @param queueCreateInfoCount
 * @param pQueueCreateInfos
 * @param enabledExtensionCount
 * @param ppEnabledExtensionNames
 * @param enabledLayerCount
 * @param ppEnabledLayerNames
 * @param pEnabledFeatures
 * @param pNext
 * @return VkDeviceCreateInfo
 */
VkDeviceCreateInfo
makeDeviceCreateInfo(uint32_t queueCreateInfoCount,
                     const VkDeviceQueueCreateInfo *pQueueCreateInfos,
                     uint32_t enabledExtensionCount = 0,
                     const char *const *ppEnabledExtensionNames = nullptr,
                     const VkPhysicalDeviceFeatures *pEnabledFeatures = nullptr,
                     uint32_t enabledLayerCount = 0,
                     const char *const *ppEnabledLayerNames = nullptr,
                     const void *pNext = nullptr);

/**
 * @brief
 *
 * @param queueFamilyIndex
 * @param queueCount
 * @param pQueuePriorities
 * @param pNext
 * @return VkDeviceQueueCreateInfo
 */
VkDeviceQueueCreateInfo makeDeviceQueueCreateInfo(uint32_t queueFamilyIndex,
                                                  uint32_t queueCount,
                                                  const float *pQueuePriorities,
                                                  const void *pNext = nullptr);

} /* namespace vkh */

#endif /* VK_MAKE_INFO_HPP */