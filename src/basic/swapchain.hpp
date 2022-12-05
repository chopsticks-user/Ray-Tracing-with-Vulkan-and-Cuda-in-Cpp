#ifndef RTVC_BASIC_SWAPCHAIN_HPP
#define RTVC_BASIC_SWAPCHAIN_HPP

#include "config.hpp"

namespace rtvc {

SwapchainWrapper makeSwapchain(
    const vk::raii::SurfaceKHR &surface, const DeviceWrapper &device,
    vk::PresentModeKHR preferredPresentMode = vk::PresentModeKHR::eImmediate);

} /* namespace rtvc */
#endif /* RTVC_BASIC_SWAPCHAIN_HPP */