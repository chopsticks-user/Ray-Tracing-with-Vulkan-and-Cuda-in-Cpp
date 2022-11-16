#ifndef VKH_MAKE_INFO_HPP
#define VKH_MAKE_INFO_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
                       uint32_t enabledLayerCount = 0,
                       const char *const *ppEnabledLayerNames = nullptr,
                       const void *pNext = nullptr);
} // namespace vkh

#endif /* VK_MAKE_INFO_HPP */