#ifndef NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP
#define NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP

#include "CoreNeko.hpp"

#include "Buffers.hpp"
#include "Images.hpp"

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

  DepthBuffer(const EngineConfigs &crSettings, const Device &crDevice,
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

  StagingBuffer(const Device &crDevice, const void *pHostData,
                VkDeviceSize bufferSize, VkDeviceSize memoryOffset = 0);

private:
};

class VertexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  VertexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
               const void *pHostData, VkDeviceSize bufferSize,
               VkDeviceSize memoryOffset = 0);

private:
};

class IndexBuffer : public BufferObject {
public:
  using BufferObject::BufferObject;

  IndexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
              const void *pHostData, VkDeviceSize bufferSize,
              VkDeviceSize memoryOffset = 0);

private:
};

class TextureImage : public ImageObject {
public:
  using ImageObject::ImageObject;

  TextureImage(std::string textureImagePath, const Device &crDevice,
               const Swapchain &crSwapchain, const CommandPool &crCommandPool,
               VkDeviceSize memoryOffset = 0);

private:
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_USAGE_SPECIFIC_HPP */