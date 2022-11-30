#ifndef RTVC_INSTANCE_HPP
#define RTVC_INSTANCE_HPP

#include <vkw.hpp>

namespace rtvc {

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData);

class Instance : public vkw::Instance {
public:
  using vkw::Instance::Instance;

  Instance() { _initialize(); }

private:
  void _initialize();
};

} /* namespace rtvc */
#endif /* RTVC_INSTANCE_HPP */