#ifndef VKW_WINDOW_HPP
#define VKW_WINDOW_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class GLFW {
public:
  GLFW() { glfwInit(); }
  ~GLFW() { glfwTerminate(); }
  GLFW(const GLFW &) = delete;
  GLFW(GLFW &&) = delete;
  GLFW &operator=(const GLFW &) = delete;
  GLFW &operator=(GLFW &&) = delete;
};

class Window {
  typedef GLFWwindow *pGLFWwindow;

public:
  Window() : _window{vkh::createWindow(800, 600)} { _isOwner = true; };
  Window(int width, int height, const char *title = "Vulkan Application",
         bool resizable = true, GLFWmonitor *monitor = nullptr,
         GLFWwindow *share = nullptr)
      : _window{vkh::createWindow(width, height, title, resizable, monitor,
                                  share)} {
    _isOwner = true;
  }
  Window(const Window &) = delete;
  Window(Window &&rhs) { _moveDataFrom(std::move(rhs)); }
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Window() { _destroyVkData(); }
  const pGLFWwindow &ref() const noexcept { return _window; }

private:
  pGLFWwindow _window;
  bool _isOwner = false;

  void _moveDataFrom(Window &&rhs) {
    _window = rhs._window;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyWindow(_window);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Window destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */

#endif /* VKW_WINDOW_HPP */