#ifndef NEKO_RENDERER_BASIC_CONTEXT_HPP
#define NEKO_RENDERER_BASIC_CONTEXT_HPP

#include "utils.hpp"

namespace neko {

class Context {
public:
  Context() { createContext(); }

  Context(const Context &) = delete;

  Context(Context &&) noexcept { moveCreateContext(); }

  Context &operator=(const Context &) = delete;

  Context &operator=(Context &&) = default;

  ~Context() { destroyContext(); }

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
  static void createContext();

  static void moveCreateContext();

  static void destroyContext();
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_CONTEXT_HPP */