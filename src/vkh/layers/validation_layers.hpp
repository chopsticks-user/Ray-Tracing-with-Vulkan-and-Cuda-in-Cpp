#ifndef VKH_VALIDATION_LAYERS_HPP
#define VKH_VALIDATION_LAYERS_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <iostream>
#include <stdexcept>
#include <vector>
// #include <string>

namespace vkh {

inline constexpr std::string_view validationLayerName =
    "VK_LAYER_KHRONOS_validation";

bool checkValidationLayerSupport();

/**
 * @brief
 *
 * @param messageSeverity
 * @param messageType
 * @param pCallbackData
 * @param pUserData
 * @return VKAPI_ATTR
 */
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

/**
 * @brief Create a Debug Utils Messenger E X T object
 *
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pDebugMessenger
 * @return VkResult
 */
VkResult createDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger);

/**
 * @brief Create a Debug Messenger object
 *
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @return VkDebugUtilsMessengerEXT
 */
VkDebugUtilsMessengerEXT
createDebugMessenger(VkInstance instance,
                     const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                     const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param instance
 * @param debugMessenger
 * @param pAllocator
 */
void destroyDebugMessenger(VkInstance instance,
                           VkDebugUtilsMessengerEXT debugMessenger,
                           const VkAllocationCallbacks *pAllocator);

} /* namespace vkh */

#endif /* VKH_VALIDATION_LAYERS_HPP */