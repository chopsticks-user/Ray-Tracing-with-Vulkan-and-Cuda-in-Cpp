#ifndef VKW_SURFACE_HPP
#define VKW_SURFACE_HPP

#include "config.hpp"

namespace vkw {

class Surface {
public:
  Surface() = default;
  Surface(VkInstance instance, GLFWwindow *window,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _window{window}, _pAllocator{pAllocator} {
    if (glfwCreateWindowSurface(instance, window, pAllocator, &_surface) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create window surface.");
    }
    _isOwner = true;
  }
  Surface(const Surface &) = delete;
  Surface(Surface &&rhs) { _moveDataFrom(std::move(rhs)); }
  Surface &operator=(const Surface &) = delete;
  Surface &operator=(Surface &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Surface() { _destroyVkData(); }

  const VkSurfaceKHR &ref() const noexcept { return _surface; }

protected:
  VkSurfaceKHR _surface = VK_NULL_HANDLE;
  VkInstance _instance = VK_NULL_HANDLE;
  GLFWwindow *_window = nullptr;
  const VkAllocationCallbacks *_pAllocator = nullptr;
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