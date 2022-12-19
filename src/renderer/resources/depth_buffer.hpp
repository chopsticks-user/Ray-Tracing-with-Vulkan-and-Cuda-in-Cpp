#ifndef NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP
#define NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP

#include "core.hpp"

#include "image.hpp"

namespace neko {

class Swapchain;

class DepthBuffer : public ImageObject {
public:
  using ImageObject::ImageObject;

  DepthBuffer(const Settings &crSettings, const Device &crDevice,
              const Swapchain &crSwapchain);

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP */