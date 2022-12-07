#ifndef NEKO_RENDERER_BASIC_CONTEXT_HPP
#define NEKO_RENDERER_BASIC_CONTEXT_HPP

#include "settings/settings.hpp"

namespace neko {

class Context {
public:
  Context() { glfwInit(); }
  ~Context() { glfwTerminate(); }

  VkResult
  createDebugMessenger(VkInstance instance,
                       const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                       const VkAllocationCallbacks *pAllocator,
                       VkDebugUtilsMessengerEXT *pMessenger);

  void destroyDebugMessenger(VkInstance instance,
                             VkDebugUtilsMessengerEXT debugMessenger,
                             const VkAllocationCallbacks *pAllocator);

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_CONTEXT_HPP */