#ifndef RTVC_SWAPCHAIN_HPP
#define RTVC_SWAPCHAIN_HPP

#include "device.hpp"
#include "instance.hpp"

namespace rtvc {

class Swapchain : public vkw::Swapchain {
public:
  using vkw::Swapchain::Swapchain;

  Swapchain(const Surface &surface, const Device &device,
            VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR) {
    _initialize(surface.ref(), device.ref(), device.physical(),
                preferredPresentMode);
  }

private:
  void _initialize(VkSurfaceKHR surface, VkDevice device,
                   VkPhysicalDevice physicalDevice,
                   VkPresentModeKHR preferredPresentMode);
};

} /* namespace rtvc */
#endif /* RTVC_SWAPCHAIN_HPP */