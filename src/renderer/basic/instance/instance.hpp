#ifndef NEKO_RENDERDER_BASIC_INSTANCE_HPP
#define NEKO_RENDERDER_BASIC_INSTANCE_HPP

#include "settings.hpp"

namespace neko {

class Context;

class Instance {
public:
  Instance() = delete;
  explicit Instance(const Settings &settings, const Context &context);
  Instance(const Instance &) = delete;
  Instance(Instance &&) = delete;
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&) = delete;
  ~Instance();

  const VkInstance &operator*() const noexcept { return mInstance; }

  std::vector<const char *> getRequiredExtensions();

private:
  const Context &mcrContext;
  VkInstance mInstance;
  VkDebugUtilsMessengerEXT mDebugMessenger;

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
      [[maybe_unused]] void *pUserData);

  void populateDebugMessengerInfo(
      VkDebugUtilsMessengerCreateInfoEXT &debugMessengerInfo);
};

} /* namespace neko */

#endif /* NEKO_RENDERDER_BASIC_INSTANCE_HPP */