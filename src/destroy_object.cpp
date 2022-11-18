#include "destroy_object.hpp"

namespace vkh {

void destroyWindow(GLFWwindow *window) { glfwDestroyWindow(window); }

void destroyInstance(VkInstance instance,
                     const VkAllocationCallbacks *pAllocator) {
  vkDestroyInstance(instance, pAllocator);
}

void destroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
  vkDestroyDevice(device, pAllocator);
}

void destroySurface(VkInstance instance, VkSurfaceKHR surface,
                    const VkAllocationCallbacks *pAllocator) {
  vkDestroySurfaceKHR(instance, surface, pAllocator);
}

void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain,
                      const VkAllocationCallbacks *pAllocator) {
  vkDestroySwapchainKHR(device, swapchain, pAllocator);
}

} /* namespace vkh */