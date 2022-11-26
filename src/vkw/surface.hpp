#ifndef VKW_SURFACE_HPP
#define VKW_SURFACE_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class Surface {
public:
  Surface() = default;
  Surface(VkInstance instance, GLFWwindow *window,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _window{window}, _pAllocator{pAllocator} {
    _surface = vkh::createSurface(instance, window, pAllocator);
    _isOwner = true;
  }
  Surface(const Surface &) = delete;
  Surface(Surface &&rhs) { _moveDataFrom(std::move(rhs)); }
  Surface &operator=(const Surface &) = delete;
  Surface &operator=(Surface &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Surface() { _destroyVkData(); }
  const VkSurfaceKHR &ref() const noexcept { return _surface; }

private:
  VkSurfaceKHR _surface;
  VkInstance _instance;
  GLFWwindow *_window;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Surface &&rhs) {
    _destroyVkData();
    _surface = rhs._surface;
    _instance = rhs._instance;
    _window = rhs._window;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroySurface(_instance, _surface, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Surface destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_SURFACE_HPP */