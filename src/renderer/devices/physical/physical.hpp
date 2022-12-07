#ifndef NEKO_RENDERER_DEVICES_PHYSICAL_HPP
#define NEKO_RENDERER_DEVICES_PHYSICAL_HPP

#include "settings.hpp"

namespace neko {

class Instance;

class PhysicalDevices {
public:
  PhysicalDevices() = delete;
  explicit PhysicalDevices(const Instance &crInstance);
  PhysicalDevices(const PhysicalDevices &) = delete;
  PhysicalDevices(PhysicalDevices &&) = delete;
  PhysicalDevices &operator=(const PhysicalDevices &) = delete;
  PhysicalDevices &operator=(PhysicalDevices &&) = delete;
  ~PhysicalDevices() = default;

  bool isIncompelete() const noexcept { return incomplete; }

  size_t count() const { return mPhysicalDevices.size(); }

  const VkPhysicalDevice &operator[](size_t index) const {
    return mPhysicalDevices.at(index);
  }

  [[nodiscard]] std::vector<VkPhysicalDeviceProperties>
  getProperties(VkPhysicalDeviceProperties properties) const;

  [[nodiscard]] std::vector<VkPhysicalDeviceProperties2>
  getProperties(VkPhysicalDeviceProperties2 properties) const;

private:
  std::vector<VkPhysicalDevice> mPhysicalDevices;
  bool incomplete;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_DEVICES_PHYSICAL_HPP */