#ifndef VKH_GET_LIST_HPP
#define VKH_GET_LIST_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <optional>
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
 * @return std::vector<const char *>
 */
std::vector<const char *>
getRequiredInstanceExtensionNameList(bool enableValidationLayers = false);

/**
 * @brief Get the Available Instance Layer List object
 *
 * @return std::vector<VkLayerProperties>
 */
std::vector<VkLayerProperties> getAvailableInstanceLayerList();

/**
 * @brief Get the Physical Device List object
 *
 * @param instance
 * @return std::vector<VkPhysicalDevice>
 */
std::vector<VkPhysicalDevice> getPhysicalDeviceList(VkInstance instance);

/**
 * @brief Get the Physical Device Property List object
 *
 * @param physicalDevice
 * @return VkPhysicalDeviceProperties
 */
VkPhysicalDeviceProperties
getPhysicalDevicePropertyList(VkPhysicalDevice physicalDevice);

/**
 * @brief Get the Physical Device Property List object
 *
 * @param physicalDevice
 * @param pNextProperty
 * @return VkPhysicalDeviceProperties2
 */
VkPhysicalDeviceProperties2
getPhysicalDevicePropertyList(VkPhysicalDevice physicalDevice,
                              void *pNextProperty);

/**
 * @brief Get the Physical Device Queue Property List object
 *
 * @param physicalDevice
 * @return std::vector<VkQueueFamilyProperties>
 */
std::vector<VkQueueFamilyProperties>
getPhysicalDeviceQueueFamilyPropertyList(VkPhysicalDevice physicalDevice);

std::vector<std::optional<VkQueueFamilyProperties>>
getGraphicsQueueFamilyPropertyList(VkPhysicalDevice physicalDevice);

std::vector<std::optional<VkQueueFamilyProperties>>
getComputeQueueFamilyPropertyList(VkPhysicalDevice physicalDevice);

std::vector<std::optional<VkQueueFamilyProperties>>
getPresentQueueFamilyPropertyList(VkPhysicalDevice physicalDevice,
                                  VkSurfaceKHR surface);

} /* namespace vkh */

#endif /* VK_GET_LIST_HPP */