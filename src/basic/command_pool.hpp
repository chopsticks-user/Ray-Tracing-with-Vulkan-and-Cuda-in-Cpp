#ifndef RTVC_BASIC_COMMAND_POOL_HPP
#define RTVC_BASIC_COMMAND_POOL_HPP

#include "config.hpp"

namespace rtvc {

vk::raii::CommandPool makeCommandPool(const DeviceWrapper &device);

vk::raii::CommandBuffers makeCommandBuffers(
    const vk::raii::Device &device, const vk::raii::CommandPool &commandPool,
    uint32_t count,
    vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

} /* namespace rtvc */
#endif /* RTVC_BASIC_COMMAND_POOL_HPP */