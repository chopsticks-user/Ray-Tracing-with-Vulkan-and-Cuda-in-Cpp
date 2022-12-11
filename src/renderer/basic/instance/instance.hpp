#ifndef NEKO_RENDERDER_BASIC_INSTANCE_HPP
#define NEKO_RENDERDER_BASIC_INSTANCE_HPP

#include "utils.hpp"

#include "renderer/basic/context/context.hpp"

namespace neko {

class Context;

class Instance {
public:
  Instance() = default;
  explicit Instance(const Settings &settings);
  Instance(const Instance &) = delete;
  Instance(Instance &&rhs) noexcept;
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&rhs);
  ~Instance() { release(); }

  const VkInstance &operator*() const noexcept { return mInstance; }

  std::vector<const char *> getRequiredExtensions();

  void release() noexcept;

private:
  Context mContext = {};
  VkInstance mInstance = nullptr;
  VkDebugUtilsMessengerEXT mDebugMessenger = nullptr;
  bool mIsOwner = false;

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