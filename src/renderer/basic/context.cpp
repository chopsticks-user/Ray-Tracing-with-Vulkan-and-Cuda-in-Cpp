#include "context.hpp"

#include <atomic>

namespace neko {

static std::atomic<u32> contextCount = 0;

void Context::createContext() {
  if (glfwInit() != GLFW_TRUE) {
    throw std::runtime_error(
        "A bug or configuration error in GLFW, the underlying operating "
        "system or its drivers, or a lack of required resources.");
  }
  ++contextCount;
}

void Context::moveCreateContext() { ++contextCount; }

void Context::destroyContext() {
  --contextCount;
  if (contextCount == 0) {
    glfwTerminate();
  }
}

VkResult Context::createDebugMessenger(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pMessenger) const {
  auto pFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (pFunc != nullptr) {
    return pFunc(instance, pCreateInfo, pAllocator, pMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void Context::destroyDebugMessenger(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) const noexcept {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

} /* namespace neko */