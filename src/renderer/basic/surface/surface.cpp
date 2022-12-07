#include "surface.hpp"

#include "renderer/basic/instance/instance.hpp"
#include "renderer/basic/window/window.hpp"

namespace neko {

Surface::Surface(const Instance &crInstance, const Window &crWindow)
    : mcrInstance{crInstance} {
  if (glfwCreateWindowSurface(*crInstance, *crWindow, nullptr, &mSurface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface.");
  }
}

Surface::~Surface() { vkDestroySurfaceKHR(*mcrInstance, mSurface, nullptr); }

} /* namespace neko */