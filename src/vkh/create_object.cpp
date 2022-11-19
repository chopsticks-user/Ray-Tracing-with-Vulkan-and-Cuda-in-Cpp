#include "create_object.hpp"

namespace vkh {
GLFWwindow *createWindow(int width, int height, const char *title,
                         GLFWmonitor *monitor, GLFWwindow *share) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  return glfwCreateWindow(width, height, title, monitor, share);
}

VkInstance createInstance(const VkInstanceCreateInfo *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator) {
  VkInstance instance;
  if (vkCreateInstance(pCreateInfo, pAllocator, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create an instance.");
  }
  return instance;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice,
                      const VkDeviceCreateInfo *pCreateInfo,
                      const VkAllocationCallbacks *pAllocator) {
  VkDevice device;
  if (vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, &device) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device.");
  }
  return device;
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window,
                           const VkAllocationCallbacks *allocator) {
  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(instance, window, allocator, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface.");
  }
  return surface;
}

VkSwapchainKHR createSwapchain(VkDevice device,
                               const VkSwapchainCreateInfoKHR *pCreateInfo,
                               const VkAllocationCallbacks *pAllocator) {
  VkSwapchainKHR swapchain{};
  if (vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, &swapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create a swapchain.");
  }
  return swapchain;
}

} /* namespace vkh */