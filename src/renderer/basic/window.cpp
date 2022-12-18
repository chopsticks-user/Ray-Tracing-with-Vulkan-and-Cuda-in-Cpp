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
    : mWindow{std::move(rhs.mWindow)}, mWidth{std::exchange(rhs.mWidth, 0)},
      mHeight{std::exchange(rhs.mHeight, 0)}, mIsOwner{std::exchange(
                                                  rhs.mIsOwner, false)} {}

Window &Window::operator=(Window &&rhs) noexcept {
  release();
  mWindow = std::move(rhs.mWindow);
  mWidth = std::exchange(rhs.mWidth, 0);
  mHeight = std::exchange(rhs.mHeight, 0);
  mIsOwner = std::exchange(rhs.mIsOwner, false);
  return *this;
}

void Window::release() noexcept {
  if (mIsOwner) {
    glfwDestroyWindow(mWindow);
    mIsOwner = false;
  }
}

void Window::open() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
  }
}

} /* namespace neko */