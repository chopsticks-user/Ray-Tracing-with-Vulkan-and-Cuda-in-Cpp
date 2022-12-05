#include "command_pool.hpp"

namespace rtvc {

vk::raii::CommandPool makeCommandPool(const DeviceWrapper &device) {
  vk::CommandPoolCreateInfo commandPoolInfo{
      .flags{vk::CommandPoolCreateFlagBits::eResetCommandBuffer},
      .queueFamilyIndex{device.queueFamilyIndex},
  };
  return {device.logical, commandPoolInfo};
}

} /* namespace rtvc */