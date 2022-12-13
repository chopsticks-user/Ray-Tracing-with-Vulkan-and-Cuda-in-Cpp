#include "window.hpp"

namespace neko {

Window::Window(const Settings &settings)
    : mWidth{settings.graphics.screenWidth},
      mHeight{settings.graphics.screenHeight} {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  mWindow = glfwCreateWindow(mWidth, mHeight, settings.general.appName.c_str(),
                             nullptr, nullptr);
}

Window::~Window() { glfwDestroyWindow(mWindow); }

void Window::open() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
  }
}

} /* namespace neko */