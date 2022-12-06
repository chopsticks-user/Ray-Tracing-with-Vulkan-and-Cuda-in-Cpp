#include "swapchain.hpp"

namespace rtvc {

vk::PresentModeKHR selectPresentMode(
    const Settings &settings,
    const std::vector<vk::PresentModeKHR> &availablePresentModes) {
  for (auto &presentMode : availablePresentModes) {
    if (presentMode == settings.presentMode) {
      return presentMode;
    }
  }
  return vk::PresentModeKHR::eFifo;
}

SwapchainWrapper makeSwapchain(const Settings &settings,
                               const vk::raii::SurfaceKHR &surface,
                               const DeviceWrapper &device) {
  auto surfaceCapabilities =
      device.physical.getSurfaceCapabilitiesKHR(*surface);
  auto surfaceFormats = device.physical.getSurfaceFormatsKHR(*surface);
  auto surfacePresentModes =
      device.physical.getSurfacePresentModesKHR(*surface);

  uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;
  if (surfaceCapabilities.maxImageCount > 0 &&
      minImageCount > surfaceCapabilities.maxImageCount) {
    minImageCount = surfaceCapabilities.maxImageCount;
  }

  if (surfaceFormats.empty()) {
    throw std::runtime_error("No surface format available");
  }
  vk::Format selectedImageFormat = surfaceFormats[0].format;
  vk::ColorSpaceKHR selectedColorSpace = surfaceFormats[0].colorSpace;
  for (size_t i = 1; i < surfaceFormats.size(); ++i) {
    if (surfaceFormats[i].format == vk::Format::eR8G8B8A8Srgb &&
        surfaceFormats[i].colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      selectedImageFormat = surfaceFormats[i].format;
      selectedColorSpace = surfaceFormats[i].colorSpace;
      break;
    }
  }

  vk::Extent2D selectedImageExtent = surfaceCapabilities.currentExtent;
  if (selectedImageExtent.width > surfaceCapabilities.currentExtent.width ||
      selectedImageExtent.height > surfaceCapabilities.currentExtent.height) {
    selectedImageExtent = surfaceCapabilities.maxImageExtent;
  }

  vk::SurfaceTransformFlagBitsKHR selectedTransform;
  if (surfaceCapabilities.supportedTransforms &
      vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    selectedTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
  } else {
    throw std::runtime_error("Identity transform not supported");
  }

  vk::CompositeAlphaFlagBitsKHR selectedCompositeAlpha;
  if (surfaceCapabilities.supportedCompositeAlpha &
      vk::CompositeAlphaFlagBitsKHR::eOpaque) {
    selectedCompositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  } else {
    throw std::runtime_error("Opaque composite alpha not supported");
  }

  vk::SwapchainCreateInfoKHR swapchainInfo{
      .surface = *surface,
      .minImageCount = minImageCount,
      .imageFormat = selectedImageFormat,
      .imageColorSpace = selectedColorSpace,
      .imageExtent = selectedImageExtent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
      .preTransform = selectedTransform,
      .compositeAlpha = selectedCompositeAlpha,
      .presentMode = selectPresentMode(settings, surfacePresentModes),
      .clipped = VK_TRUE,
      .oldSwapchain = VK_NULL_HANDLE,
  };
  return {
      .self = {device.logical, swapchainInfo},
      .format = selectedImageFormat,
      .extent = selectedImageExtent,
  };
}

} /* namespace rtvc */