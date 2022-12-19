#ifndef NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP
#define NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP

#include "core.hpp"

namespace neko {

class DepthBuffer {
public:
private:
  VkImage mDepthImage = VK_NULL_HANDLE;
  VkImageView mDepthImageView = VK_NULL_HANDLE;
  bool mIsOwner = false;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_DEPTH_BUFFER_HPP */