#ifndef RTVC_BUFFER_DEPTH_HPP
#define RTVC_BUFFER_DEPTH_HPP

#include "config.hpp"

#include <vector>

namespace rtvc {

vk::Format findSupportedFormat(const vk::raii::PhysicalDevice physicalDevice,
                               const std::vector<vk::Format> &candidates,
                               vk::ImageTiling tiling,
                               vk::FormatFeatureFlags features);

DepthBufferWrapper makeDepthBuffer(const Settings &settings,
                                   const DeviceWrapper &device,
                                   const SwapchainWrapper &swapchain);

} /* namespace rtvc */
#endif /* RTVC_BUFFER_DEPTH_HPP */