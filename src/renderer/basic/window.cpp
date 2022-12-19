#include "window.hpp"

#include <iostream>

namespace neko {

Window::Window(const Settings &settings)
    : mWidth{settings.graphics.screenWidth},
      mHeight{settings.graphics.screenHeight} {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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

void Window::release() noexcept {
  if (mWindow != nullptr) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

void Window::open() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
  }
}

} /* namespace neko */