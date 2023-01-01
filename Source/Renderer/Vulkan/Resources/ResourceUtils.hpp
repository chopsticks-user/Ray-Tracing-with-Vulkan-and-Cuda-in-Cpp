#ifndef HDG_RENDERER_VULKAN_RESOURCES_UTILS_HPP
#define HDG_RENDERER_VULKAN_RESOURCES_UTILS_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

namespace Neko::Internal::VK
{
    class CommandPool;
    class BufferObject;
    class Device;

    bool hasDepthStencilComponent(VkFormat format);

    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice,
                                 const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    void transitImageLayout(const CommandPool &commandPool, VkImage image,
                            VkFormat format, VkImageLayout oldLayout,
                            VkImageLayout newLayout, u32 mipLevels = 1);

    void copyBufferToImage(const CommandPool &commandPool, VkBuffer buffer,
                           VkImage image, u32 width, u32 height);

    VkSampleCountFlagBits maxSampleCount(VkPhysicalDevice physicalDevice);

    void generateMipMaps(const CommandPool &commandPool,
                         VkPhysicalDevice physicalDevice, VkImage image,
                         VkFormat format, i32 width, i32 height, u32 mipLevels);

    u32 findMemoryType(VkPhysicalDevice physicalDevice, u32 typeFilter,
                       VkMemoryPropertyFlags propertyFlags);

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_RESOURCES_UTILS_HPP */