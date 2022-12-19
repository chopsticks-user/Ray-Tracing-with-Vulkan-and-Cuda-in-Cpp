#ifndef NEKO_RENDERER_RESOURCES_UTILS_HPP
#define NEKO_RENDERER_RESOURCES_UTILS_HPP

#include "core.hpp"

namespace neko {

class CommandPool;
class BufferObject;

namespace detail {

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
                   VkMemoryPropertyFlags propFlags);

} /* namespace detail */
} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_UTILS_HPP */