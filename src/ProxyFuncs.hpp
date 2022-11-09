#ifndef RTVC_PROXY_FUNCS_HPP
#define RTVC_PROXY_FUNCS_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace rtvc
{
    VkResult createDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *,
                                          const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);

    void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT,
                                       const VkAllocationCallbacks *);
} // namespace rtvc

#endif // RTVC_PROXY_FUNCS_HPP