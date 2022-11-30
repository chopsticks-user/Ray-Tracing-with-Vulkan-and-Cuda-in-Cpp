#ifndef RTVC_DEVICE_HPP
#define RTVC_DEVICE_HPP

#include "instance.hpp"

#include <vkw.hpp>

#include <map>
#include <optional>

namespace rtvc {

class Device : public vkw::Device {
public:
  using vkw::Device::Device;

  Device(const Instance &instance, const vkw::Surface &surface);

private:
  void _initialize(VkInstance instance, VkSurfaceKHR surface);

  bool _checkProperties(VkPhysicalDevice physicalDevice);

  bool _checkSwapchainSupport([[maybe_unused]] VkPhysicalDevice physicalDevice);

  bool
  _checkExtensionSupport(VkPhysicalDevice physicalDevice,
                         const std::vector<const char *> &deviceExtensions);

  bool _checkFeatureSupport(VkPhysicalDevice physicalDevice);

  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
  _selectQueueFamily(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
};

} /* namespace rtvc */
#endif /* RTVC_DEVICE_HPP */