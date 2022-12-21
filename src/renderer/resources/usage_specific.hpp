#ifndef NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP
#define NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP

#include "core.hpp"

#include "buffer.hpp"
#include "image.hpp"

namespace neko {

class Swapchain;
class Device;

class DepthBuffer : public ImageObject {
public:
  using ImageObject::ImageObject;

  DepthBuffer(DepthBuffer &&rhs)
      : ImageObject{static_cast<ImageObject>(std::move(rhs))},
        mFormat{std::move(rhs.mFormat)} {}

  DepthBuffer &operator=(DepthBuffer &&rhs) {
    ImageObject::operator=(static_cast<ImageObject>(std::move(rhs)));
    mFormat = std::move(rhs.mFormat);
    return *this;
  }

  DepthBuffer(const Configs &crSettings, const Device &crDevice,
              const Swapchain &crSwapchain);

  const VkFormat &format() const noexcept { return mFormat; }

private:
  VkFormat mFormat = {};
};

class UniformBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  UniformBuffer(const Device &crDevice, VkDeviceSize bufferSize,
                VkDeviceSize memoryOffset = 0);

private:
};

class StagingBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  StagingBuffer(const Device &crDevice, void *pHostData,
                VkDeviceSize bufferSize, VkDeviceSize memoryOffset = 0);

private:
};

class VertexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  VertexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
               void *pHostData, VkDeviceSize bufferSize,
               VkDeviceSize memoryOffset = 0);

private:
};

class IndexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  IndexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
              void *pHostData, VkDeviceSize bufferSize,
              VkDeviceSize memoryOffset = 0);

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP */