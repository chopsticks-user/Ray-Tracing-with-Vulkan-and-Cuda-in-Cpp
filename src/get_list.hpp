#ifndef VKH_GET_LIST_HPP
#define VKH_GET_LIST_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace vkh {
/**
 * @brief Get the Available Instance Extension List object
 *
 * @param pLayerName
 * @return std::vector<VkExtensionProperties>
 */
std::vector<VkExtensionProperties>
getAvailableInstanceExtensionList(const char *pLayerName = nullptr);

/**
 * @brief Get the Available Device Extension List object
 *
 * @param physicalDevice
 * @param pLayerName
 * @return std::vector<VkExtensionProperties>
 */
std::vector<VkExtensionProperties>
getAvailableDeviceExtensionList(VkPhysicalDevice physicalDevice,
                                const char *pLayerName = nullptr);

/**
 * @brief Get the Required Instance Extension List object
 *
 * @return std::vector<std::string>
 */
std::vector<std::string> getRequiredInstanceExtensionList();

/**
 * @brief Get the Physical Device List object
 *
 * @param instance
 * @return std::vector<VkPhysicalDevice>
 */
std::vector<VkPhysicalDevice> getPhysicalDeviceList(VkInstance instance);

} // namespace vkh

#endif /* VK_GET_LIST_HPP */