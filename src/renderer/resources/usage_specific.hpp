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

  DepthBuffer(const Configs &crSettings, const Device &crDevice,
              const Swapchain &crSwapchain);

private:
};

class UniformBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  UniformBuffer(const Device &crDevice, VkDeviceSize bufferSize);

private:
};

class StagingBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  StagingBuffer(const Device &crDevice, void *pHostData,
                VkDeviceSize bufferSize);

private:
};

class VertexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  VertexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
               void *pHostData, VkDeviceSize bufferSize);

private:
};

class IndexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  IndexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
              void *pHostData, VkDeviceSize bufferSize);

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP */