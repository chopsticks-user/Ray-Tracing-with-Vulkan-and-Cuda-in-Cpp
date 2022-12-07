#ifndef NEKO_RENDERER_BASIC_CONTEXT_HPP
#define NEKO_RENDERER_BASIC_CONTEXT_HPP

#include "settings.hpp"

namespace neko {

class Context {
public:
  Context() {
    if (glfwInit() != GLFW_TRUE) {
      throw std::runtime_error(
          "A bug or configuration error in GLFW, the underlying operating "
          "system or its drivers, or a lack of required resources.");
    }
  }
  Context(const Context &) = delete;
  Context(Context &&) = delete;
  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) = delete;
  ~Context() { glfwTerminate(); }

  VkResult
  createDebugMessenger(VkInstance instance,
                       const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                       const VkAllocationCallbacks *pAllocator,
                       VkDebugUtilsMessengerEXT *pMessenger) const;

  void
  destroyDebugMessenger(VkInstance instance,
                        VkDebugUtilsMessengerEXT debugMessenger,
                        const VkAllocationCallbacks *pAllocator) const noexcept;

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_CONTEXT_HPP */