#include "window.hpp"

namespace neko {

Window::Window(const Configs &settings, bool windowResizeable)
    : mWidth{settings.graphics.screenWidth},
      mHeight{settings.graphics.screenHeight} {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, windowResizeable);
  mWindow = glfwCreateWindow(mWidth, mHeight, settings.general.appName.c_str(),
                             nullptr, nullptr);
}

Window::Window(Window &&rhs) noexcept
    : mWindow{std::exchange(rhs.mWindow, nullptr)},
      mWidth{std::exchange(rhs.mWidth, 0)}, mHeight{std::exchange(rhs.mHeight,
                                                                  0)} {}

Window &Window::operator=(Window &&rhs) noexcept {
  release();
  mWindow = std::exchange(rhs.mWindow, nullptr);
  mWidth = std::exchange(rhs.mWidth, 0);
  mHeight = std::exchange(rhs.mHeight, 0);
  return *this;
}

void Window::whileMinimize() noexcept {
  int width = 0, heigth = 0;
  glfwGetFramebufferSize(mWindow, &width, &heigth);
  while (width == 0 || heigth == 0) {
    glfwGetFramebufferSize(mWindow, &width, &heigth);
    glfwWaitEvents();
  }
}

void Window::release() noexcept {
  if (mWindow != nullptr) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

} /* namespace neko */