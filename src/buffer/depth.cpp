#include "depth.hpp"

namespace rtvc {

vk::Format findSupportedFormat(const vk::raii::PhysicalDevice physicalDevice,
                               const std::vector<vk::Format> &candidates,
                               vk::ImageTiling tiling,
                               vk::FormatFeatureFlags features) {
  for (auto format : candidates) {
    auto formatProperties = physicalDevice.getFormatProperties(format);
    if (tiling == vk::ImageTiling::eLinear &&
        formatProperties.linearTilingFeatures &
            vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
      return format;
    } else if (tiling == vk::ImageTiling::eOptimal &&
               formatProperties.optimalTilingFeatures &
                   vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
      return format;
    }
  }
  throw std::runtime_error("Failed to find supported format");
}

uint32_t findMemoryType(const vk::raii::Image &image) {}

DepthBufferWrapper makeDepthBuffer(const Settings &settings,
                                   const DeviceWrapper &device,
                                   const SwapchainWrapper &swapchain) {
  vk::ImageTiling depthImageTiling = vk::ImageTiling::eOptimal;

  vk::Format depthFormat = findSupportedFormat(
      device.physical,
      {
          vk::Format::eD32Sfloat,
          vk::Format::eD32SfloatS8Uint,
          vk::Format::eD24UnormS8Uint,
      },
      depthImageTiling, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

  vk::ImageCreateInfo depthImageInfo = {
      .imageType = vk::ImageType::e2D,
      .format = depthFormat,
      .extent = {swapchain.extent.width, swapchain.extent.height, 1},
      .mipLevels = 1,
      .arrayLayers = 1,
      .samples = settings.msaaSamples,
      .tiling = depthImageTiling,
      .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment,
  };

  vk::raii::Image depthImage{device.logical, depthImageInfo};

  auto memoryRequirements = depthImage.getMemoryRequirements();
  uint32_t memoryTypeBits = memoryRequirements.memoryTypeBits;
  uint32_t memoryTypeIndex = ~0;
}

} /* namespace rtvc */