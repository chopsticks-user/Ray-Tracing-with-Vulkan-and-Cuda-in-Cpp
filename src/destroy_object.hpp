#ifndef VKH_DESTROY_OBJECT_HPP
#define VKH_DESTROY_OBJECT_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
void destroyDevice(VkDevice *device,
                   const VkAllocationCallbacks *pAllocator = nullptr);

} /* namespace vkh */

#endif /* VKH_DESTROY_OBJECT_HPP */