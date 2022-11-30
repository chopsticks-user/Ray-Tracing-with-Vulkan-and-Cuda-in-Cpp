#ifndef RTVC_INSTANCE_HPP
#define RTVC_INSTANCE_HPP

#include <vkw.hpp>

namespace rtvc {

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData);

class GLFW : public vkw::GLFW {};

class Window : public vkw::Window {};

class Instance : public vkw::Instance {
public:
  using vkw::Instance::Instance;

  Instance() { _initialize(); }

private:
  void _initialize();
};

class DebugMessenger : public vkw::DebugMessenger {
public:
  using vkw::DebugMessenger::DebugMessenger;

  DebugMessenger(const Instance &instance)
      : vkw::DebugMessenger{instance.ref(), &instance.debugInfo()} {}
};

class Surface : public vkw::Surface {
public:
  using vkw::Surface::Surface;

  Surface(const Instance &instance, const vkw::Window &window)
      : vkw::Surface{instance.ref(), window.ref()} {}
};

} /* namespace rtvc */
#endif /* RTVC_INSTANCE_HPP */