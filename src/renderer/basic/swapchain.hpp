#ifndef NEKO_RENDERER_BASIC_SWAPCHAIN_HPP
#define NEKO_RENDERER_BASIC_SWAPCHAIN_HPP

#include "core.hpp"

namespace neko {

class Surface;
class Device;

class Swapchain {
public:
  Swapchain() = default;
  Swapchain(const Swapchain &) = delete;
  Swapchain &operator=(const Swapchain &) = delete;

  Swapchain(const Settings &crSettings, const Surface &crSurface,
            const Device &crDevice);

  Swapchain(Swapchain &&rhs) noexcept;

  Swapchain &operator=(Swapchain &&rhs) noexcept;

  ~Swapchain() noexcept { release(); }

  const VkSwapchainKHR &operator*() const noexcept { return mSwapchain; }

  void release() noexcept;

  const VkFormat &format() const noexcept { return mFormat; }

  const VkExtent2D &extent() const noexcept { return mExtent; }

private:
  const Device *mpcDevice = nullptr;
  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  VkFormat mFormat = {};
  VkExtent2D mExtent = {};
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_SWAPCHAIN_HPP */