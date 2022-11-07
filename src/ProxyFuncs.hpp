#ifndef RT_VULKAN_PROXY_FUNCS_HPP
#define RT_VULKAN_PROXY_FUNCS_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace rt_vulkan
{
    VkResult CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *,
                                          const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);

    void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT,
                                       const VkAllocationCallbacks *);
} // namespace rt_vulkan

#endif // RT_VULKAN_PROXY_FUNCS_HPP