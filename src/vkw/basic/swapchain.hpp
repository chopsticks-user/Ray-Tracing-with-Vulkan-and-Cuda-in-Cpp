#ifndef VKW_SWAPCHAIN_HPP
#define VKW_SWAPCHAIN_HPP

#include "config.hpp"

namespace vkw {

class Swapchain {
public:
  Swapchain() = default;
  Swapchain(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
            const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, &_swapchain);
    _format = pCreateInfo->imageFormat;
    _extent = pCreateInfo->imageExtent;
    _isOwner = true;
  }
  Swapchain(const Swapchain &) = delete;
  Swapchain(Swapchain &&rhs) { _moveDataFrom(std::move(rhs)); }
  Swapchain &operator=(const Swapchain &) = delete;
  Swapchain &operator=(Swapchain &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Swapchain() { _destroyVkData(); }

  const VkSwapchainKHR &ref() const noexcept { return _swapchain; }

  const VkFormat &format() const noexcept { return _format; }

  const VkExtent2D &extent() const noexcept { return _extent; }

protected:
  VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
  VkFormat _format = {};
  VkExtent2D _extent = {};
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Swapchain &&rhs) {
    _destroyVkData();
    _swapchain = rhs._swapchain;
    _format = std::move(rhs._format);
    _extent = std::move(rhs._extent);
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroySwapchainKHR(_device, _swapchain, _pAllocator);
      _isOwner = false;
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_SWAPCHAIN_HPP */