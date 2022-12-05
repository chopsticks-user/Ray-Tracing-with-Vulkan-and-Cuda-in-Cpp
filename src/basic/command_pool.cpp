#include "command_pool.hpp"

namespace rtvc {

vk::raii::CommandPool makeCommandPool(const DeviceWrapper &device) {
  vk::CommandPoolCreateInfo commandPoolInfo{
      .flags{vk::CommandPoolCreateFlagBits::eResetCommandBuffer},
      .queueFamilyIndex{device.queueFamilyIndex},
  };
  return {device.logical, commandPoolInfo};
}

vk::raii::CommandBuffers
makeCommandBuffers(const vk::raii::Device &device,
                   const vk::raii::CommandPool &commandPool, uint32_t count,
                   vk::CommandBufferLevel level) {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool{*commandPool},
      .level{level},
      .commandBufferCount{count},
  };
  return {device, allocInfo};
}

} /* namespace rtvc */