#ifndef NEKO_RENDERER_DEVICES_LOGICAL_HPP
#define NEKO_RENDERER_DEVICES_LOGICAL_HPP

#include "settings.hpp"

namespace neko {

class Instance;
class Surface;

/**
 * @brief
 *
 * TODO: Temporary implementation, the members should not be modified
 *
 */
struct UniversalQueue {
  uint32_t mFamilyIndex;
  uint32_t mIndex;
  VkQueue mQueue;
};

/**
 * @brief
 *
 * !Use only one universal queue
 */
class LogicalDevice : public StaticObject {
public:
  using StaticObject::StaticObject;

  LogicalDevice(const Instance &crInstance, const Surface &crSurface);

  ~LogicalDevice();

  const VkDevice &operator*() const noexcept { return mLogicalDevice; }

  const VkPhysicalDevice &physical() const noexcept { return mPhysicalDevice; }

  const UniversalQueue &queue() const noexcept { return mQueue; }

private:
  VkDevice mLogicalDevice;
  VkPhysicalDevice mPhysicalDevice;
  UniversalQueue mQueue;

  uint32_t selectUniversalQueueFamily(VkPhysicalDevice physicalDevice,
                                      VkSurfaceKHR surface);

  [[nodiscard]] VkPhysicalDevice
  selectPhysicalDevice(std::vector<VkPhysicalDevice> &&rrPhysicalDevices);

  bool checkRequirements(VkPhysicalDevice physicalDevice);

  bool checkProperties(VkPhysicalDevice physicalDevice);

  bool checkExtensions(VkPhysicalDevice physicalDevice);

  bool checkFeatures(VkPhysicalDevice physicalDevice);

  std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
};

} /* namespace neko */

#endif /* NEKO_RENDERER_DEVICES_LOGICAL_HPP */