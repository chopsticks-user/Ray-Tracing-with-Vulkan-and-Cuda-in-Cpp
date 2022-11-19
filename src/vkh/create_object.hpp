#ifndef VKH_CREATE_OBJECT_HPP
#define VKH_CREATE_OBJECT_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <stdexcept>

namespace vkh {
/**
 * @brief Create a Window object
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
                         GLFWwindow *share = nullptr);

/**
 * @brief Create a Instance object
 *
 * @param pCreateInfo
 * @param pAllocator
 * @return VkInstance
 */
VkInstance createInstance(const VkInstanceCreateInfo *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Device object
 *
 * @param physicalDevice
 * @param pCreateInfo
 * @param pAllocator
 * @return VkDevice
 */
VkDevice createDevice(VkPhysicalDevice physicalDevice,
                      const VkDeviceCreateInfo *pCreateInfo,
                      const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Surface object
 *
 * @param instance
 * @param window
 * @param allocator
 * @return VkSurfaceKHR
 */
VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window,
                           const VkAllocationCallbacks *allocator = nullptr);

/**
 * @brief Create a Swapchain object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkSwapchainKHR
 */
VkSwapchainKHR
createSwapchain(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                const VkAllocationCallbacks *pAllocator = nullptr);

} /* namespace vkh */

#endif /* VK_CREATE_OBJECT_HPP */