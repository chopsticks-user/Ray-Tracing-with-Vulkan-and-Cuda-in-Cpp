#ifndef NEKO_RENDERDER_BASIC_INSTANCE_HPP
#define NEKO_RENDERDER_BASIC_INSTANCE_HPP

#include "core.hpp"

#include "context.hpp"

namespace neko {

class Context;

class Instance {
public:
  Instance() = default;
  Instance(const Instance &) = delete;
  Instance &operator=(const Instance &) = delete;

  explicit Instance(const Settings &settings);

  Instance(Instance &&rhs) noexcept;

  Instance &operator=(Instance &&rhs) noexcept;

  ~Instance() noexcept { release(); }

  const VkInstance &operator*() const noexcept { return mInstance; }

  void release() noexcept;

  std::vector<const char *> getRequiredExtensions();

private:
  Context mContext = {};
  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

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