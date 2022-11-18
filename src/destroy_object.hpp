#ifndef VKH_DESTROY_OBJECT_HPP
#define VKH_DESTROY_OBJECT_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

namespace vkh {
/**
 * @brief
 *
 * @param window
 */
void destroyWindow(GLFWwindow *window);

/**
 * @brief
 *
 * @param instance
 * @param pAllocator
 */
void destroyInstance(VkInstance instance,
                     const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param pAllocator
 */
void destroyDevice(VkDevice device,
                   const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param instance
 * @param surface
 * @param pAllocator
 */
void destroySurface(VkInstance instance, VkSurfaceKHR surface,
                    const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param swapchain
 * @param pAllocator
 */
void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain,
                      const VkAllocationCallbacks *pAllocator = nullptr);

} /* namespace vkh */

#endif /* VKH_DESTROY_OBJECT_HPP */