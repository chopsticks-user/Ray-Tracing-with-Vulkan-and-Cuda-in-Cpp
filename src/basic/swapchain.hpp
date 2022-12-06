#ifndef RTVC_BASIC_SWAPCHAIN_HPP
#define RTVC_BASIC_SWAPCHAIN_HPP

#include "config.hpp"

namespace rtvc {

vk::PresentModeKHR
selectPresentMode(const Settings &settings,
                  const std::vector<vk::PresentModeKHR> &availablePresentModes);

SwapchainWrapper makeSwapchain(const Settings &settings,
                               const vk::raii::SurfaceKHR &surface,
                               const DeviceWrapper &device);

} /* namespace rtvc */
#endif /* RTVC_BASIC_SWAPCHAIN_HPP */