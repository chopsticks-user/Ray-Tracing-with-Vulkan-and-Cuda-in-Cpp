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

} /* namespace vkh */