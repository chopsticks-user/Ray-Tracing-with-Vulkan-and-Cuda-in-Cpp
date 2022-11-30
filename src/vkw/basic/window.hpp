#ifndef VKW_BASIC_WINDOW_HPP
#define VKW_BASIC_WINDOW_HPP

#include "config.hpp"

namespace vkw {

class GLFW {
public:
  GLFW() { glfwInit(); }
  virtual ~GLFW() { glfwTerminate(); }
  GLFW(const GLFW &) = delete;
  GLFW(GLFW &&) = delete;
  GLFW &operator=(const GLFW &) = delete;
  GLFW &operator=(GLFW &&) = delete;
};

class Window {
  typedef GLFWwindow *pGLFWwindow;

public:
  Window() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window =
        glfwCreateWindow(800, 600, "Vulkan Application", nullptr, nullptr);
    _isOwner = true;
  }
  Window(int width, int height, const char *title = "Vulkan Application",
         bool resizable = true, GLFWmonitor *monitor = nullptr,
         GLFWwindow *share = nullptr) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    _window = glfwCreateWindow(width, height, title, monitor, share);
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

  void whileMinimized() {
    int width = 0, heigth = 0;
    glfwGetFramebufferSize(_window, &width, &heigth);
    while (width == 0 || heigth == 0) {
      glfwGetFramebufferSize(_window, &width, &heigth);
      glfwWaitEvents();
    }
  }

protected:
  pGLFWwindow _window = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Window &&rhs) {
    _destroyVkData();
    _window = rhs._window;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      glfwDestroyWindow(_window);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Window destructor\n";
      }
    }
  }
};

} /* namespace vkw */

#endif /* VKW_BASIC_WINDOW_HPP */