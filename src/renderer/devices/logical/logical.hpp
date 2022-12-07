#ifndef NEKO_RENDERER_DEVICES_LOGICAL_HPP
#define NEKO_RENDERER_DEVICES_LOGICAL_HPP

#include "settings.hpp"

namespace neko {

class Instance;

class LogicalDevice {
public:
  LogicalDevice() = delete;
  explicit LogicalDevice(const Instance &crInstance);
  LogicalDevice(const LogicalDevice &) = delete;
  LogicalDevice(LogicalDevice &&) = delete;
  LogicalDevice &operator=(const LogicalDevice &) = delete;
  LogicalDevice &operator=(LogicalDevice &&) = delete;
  ~LogicalDevice() = default;

private:
  VkDevice mLogicalDevice;
  VkPhysicalDevice mPhysicalDevice;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_DEVICES_LOGICAL_HPP */