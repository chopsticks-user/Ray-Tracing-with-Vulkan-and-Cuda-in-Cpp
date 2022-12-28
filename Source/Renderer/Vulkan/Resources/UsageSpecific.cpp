#include "UsageSpecific.hpp"
#include "ResourceUtils.hpp"

#include "Basic/Swapchain.hpp"
#include "Devices/Logical.hpp"

#include <cmath>

namespace Neko::Internal::VK
{

    DepthBuffer::DepthBuffer([[maybe_unused]] const Core::EngineConfigs &crSettings,
                             const Device &crDevice, const Swapchain &crSwapchain)
    {
        static constexpr u32 mipLevels = 1;
        static constexpr VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

        mpcDevice = &crDevice;
        mOffset = 0;

        mFormat = findSupportedFormat(
            crDevice.physical(),
            {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = crSwapchain.extent().width;
        imageInfo.extent.height = crSwapchain.extent().height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = mFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = samples;
        imageInfo.flags = 0;
        createImage(&imageInfo);

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(**mpcDevice, mImage, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            mpcDevice->physical(), memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        createImageMemory(&allocInfo);

        VkImageViewCreateInfo imageViewInfo{};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.pNext = nullptr;
        imageViewInfo.image = mImage;

        /* treat images as 2D textures */
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        imageViewInfo.format = mFormat;

        /* default mapping */
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        /* color aspect */
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        /* In stereographic 3D applications, create a swapchain with multiple
        layers before creating multiple image views for each images representing
        the views for the left and right eyes by accessing different layers */
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = mipLevels;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        createImageView(&imageViewInfo);
    }

    UniformBuffer::UniformBuffer(const Device &crDevice, VkDeviceSize bufferSize,
                                 VkDeviceSize memoryOffset)
        : BufferObject{crDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       memoryOffset} {}

    StagingBuffer::StagingBuffer(const Device &crDevice, const void *pHostData,
                                 VkDeviceSize bufferSize, VkDeviceSize memoryOffset)
        : BufferObject{crDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       memoryOffset}
    {
        this->copy(pHostData, bufferSize);
    }

    VertexBuffer::VertexBuffer(const Device &crDevice,
                               const CommandPool &crCommandPool,
                               const void *pHostData, VkDeviceSize bufferSize,
                               VkDeviceSize memoryOffset)
        : BufferObject{crDevice, bufferSize,
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryOffset}
    {
        StagingBuffer stagingBuffer{crDevice, pHostData, bufferSize};
        this->copy(crCommandPool, static_cast<BufferObject>(std::move(stagingBuffer)),
                   bufferSize);
    }

    IndexBuffer::IndexBuffer(const Device &crDevice,
                             const CommandPool &crCommandPool,
                             const void *pHostData, VkDeviceSize bufferSize,
                             VkDeviceSize memoryOffset)
        : BufferObject{crDevice, bufferSize,
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                           VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryOffset}
    {
        StagingBuffer stagingBuffer{crDevice, pHostData, bufferSize};
        this->copy(crCommandPool, static_cast<BufferObject>(std::move(stagingBuffer)),
                   bufferSize);
    }

    TextureImage::TextureImage(std::string textureImagePath, const Device &crDevice,
                               const CommandPool &crCommandPool,
                               VkDeviceSize memoryOffset)
    {
        static constexpr VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
        static constexpr VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

        mpcDevice = &crDevice;
        mOffset = memoryOffset;

        Core::ImageLoader textureImage{textureImagePath};
        u32 mipLevels = textureImage.mipLevels();
        VkDeviceSize textureImageSize = textureImage.imageSize();
        StagingBuffer stagingBuffer{crDevice, textureImage.pixels(),
                                    textureImageSize};

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = vku32(textureImage.width());
        imageInfo.extent.height = vku32(textureImage.height());
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = samples;
        imageInfo.flags = 0;
        createImage(&imageInfo);

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(**mpcDevice, mImage, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            mpcDevice->physical(), memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        createImageMemory(&allocInfo);

        transitImageLayout(crCommandPool, mImage, VK_FORMAT_R8G8B8A8_SRGB,
                           VK_IMAGE_LAYOUT_UNDEFINED,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);

        copyBufferToImage(crCommandPool, *stagingBuffer, mImage,
                          textureImage.width(), textureImage.height());

        generateMipMaps(crCommandPool, crDevice.physical(), mImage, format,
                        textureImage.width(), textureImage.height(),
                        mipLevels);

        VkImageViewCreateInfo imageViewInfo{};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.pNext = nullptr;
        imageViewInfo.image = mImage;

        /* treat images as 2D textures */
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        imageViewInfo.format = format;

        /* default mapping */
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        /* color aspect */
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        /* In stereographic 3D applications, create a swapchain with multiple
        layers before creating multiple image views for each images representing
        the views for the left and right eyes by accessing different layers */
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = mipLevels;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        createImageView(&imageViewInfo);
    }

} // namespace Neko::Internal::VK