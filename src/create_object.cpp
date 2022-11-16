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
    throw std::runtime_error("Failed to a create device.");
  }
  return device;
}

} /* namespace vkh */