#ifndef HDG_RENDERER_VULKAN_RESOURCES_USAGE_SPECIFIC_HPP
#define HDG_RENDERER_VULKAN_RESOURCES_USAGE_SPECIFIC_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

#include "Buffers.hpp"
#include "Images.hpp"

namespace Neko::Internal::VK
{
    class Swapchain;
    class Device;

    class DepthBuffer : public ImageObject
    {
    public:
        using ImageObject::ImageObject;

        DepthBuffer(DepthBuffer &&rhs)
            : ImageObject{static_cast<ImageObject>(std::move(rhs))},
              mFormat{std::move(rhs.mFormat)} {}

        DepthBuffer &operator=(DepthBuffer &&rhs)
        {
            ImageObject::operator=(static_cast<ImageObject>(std::move(rhs)));
            mFormat = std::move(rhs.mFormat);
            return *this;
        }

        DepthBuffer(const Core::EngineConfigs &crSettings, const Device &crDevice,
                    const Swapchain &crSwapchain);

        const VkFormat &format() const noexcept { return mFormat; }

    private:
        VkFormat mFormat = {};
    };

    class UniformBuffer : public BufferObject
    {
    public:
        using BufferObject::BufferObject;

        UniformBuffer(const Device &crDevice, VkDeviceSize bufferSize,
                      VkDeviceSize memoryOffset = 0);

    private:
    };

    class StagingBuffer : public BufferObject
    {
    public:
        using BufferObject::BufferObject;

        StagingBuffer(const Device &crDevice, const void *pHostData,
                      VkDeviceSize bufferSize, VkDeviceSize memoryOffset = 0);

    private:
    };

    class VertexBuffer : public BufferObject
    {
    public:
        using BufferObject::BufferObject;

        VertexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
                     const void *pHostData, VkDeviceSize bufferSize,
                     VkDeviceSize memoryOffset = 0);

    private:
    };

    class IndexBuffer : public BufferObject
    {
    public:
        using BufferObject::BufferObject;

        IndexBuffer(const Device &crDevice, const CommandPool &crCommandPool,
                    const void *pHostData, VkDeviceSize bufferSize,
                    VkDeviceSize memoryOffset = 0);

    private:
    };

    class TextureImage : public ImageObject
    {
    public:
        using ImageObject::ImageObject;

        TextureImage(std::string textureImagePath, const Device &crDevice,
                     const CommandPool &crCommandPool,
                     VkDeviceSize memoryOffset = 0);

    private:
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_RESOURCES_USAGE_SPECIFIC_HPP */