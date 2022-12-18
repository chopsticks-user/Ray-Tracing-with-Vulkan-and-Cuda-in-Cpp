#include "surface.hpp"

#include "instance.hpp"
#include "window.hpp"

namespace neko {

Surface::Surface(const Instance &crInstance, const Window &crWindow)
    : mpcInstance{&crInstance} {
  if (glfwCreateWindowSurface(*crInstance, *crWindow, nullptr, &mSurface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface.");
  }
  mIsOwner = true;
}

Surface::Surface(Surface &&rhs) noexcept
    : mpcInstance{std::move(rhs.mpcInstance)},
      mSurface{std::move(rhs.mSurface)}, mIsOwner{std::exchange(rhs.mIsOwner,
                                                                false)} {}

Surface &Surface::operator=(Surface &&rhs) noexcept {
  release();
  mpcInstance = std::move(rhs.mpcInstance);
  mSurface = std::move(rhs.mSurface);
  mIsOwner = std::exchange(rhs.mIsOwner, false);
  return *this;
}

void Surface::release() noexcept {
  if (mIsOwner) {
    vkDestroySurfaceKHR(**mpcInstance, mSurface, nullptr);
    mIsOwner = false;
  }
}

} /* namespace neko */