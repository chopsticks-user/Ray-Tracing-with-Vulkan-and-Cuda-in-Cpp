#ifndef NEKO_RENDERER_BASIC_SURFACE_HPP
#define NEKO_RENDERER_BASIC_SURFACE_HPP

#include "settings.hpp"

namespace neko {

class Instance;
class Window;

class Surface {
public:
  Surface() = default;

  Surface(const Instance &crInstance, const Window &crWindow);

  Surface(const Surface &) = delete;

  Surface(Surface &&) = default;

  Surface &operator=(const Surface &) = delete;

  Surface &operator=(Surface &&) = default;

  ~Surface();

  const VkSurfaceKHR &operator*() const noexcept { return mSurface; }

private:
  const Instance &mcrInstance;
  VkSurfaceKHR mSurface;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_SURFACE_HPP */