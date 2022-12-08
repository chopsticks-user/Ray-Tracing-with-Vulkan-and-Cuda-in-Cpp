#ifndef NEKO_RENDERER_BASIC_CONTEXT_HPP
#define NEKO_RENDERER_BASIC_CONTEXT_HPP

#include "settings.hpp"

namespace neko {

class Context : public StaticObject {
public:
  using StaticObject::StaticObject;

  Context() {
    if (glfwInit() != GLFW_TRUE) {
      throw std::runtime_error(
          "A bug or configuration error in GLFW, the underlying operating "
          "system or its drivers, or a lack of required resources.");
    }
  }

  virtual ~Context() { glfwTerminate(); }

  VkResult
  createDebugMessenger(VkInstance instance,
                       const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                       const VkAllocationCallbacks *pAllocator,
                       VkDebugUtilsMessengerEXT *pMessenger) const;

  void
  destroyDebugMessenger(VkInstance instance,
                        VkDebugUtilsMessengerEXT debugMessenger,
                        const VkAllocationCallbacks *pAllocator) const noexcept;

protected:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_CONTEXT_HPP */