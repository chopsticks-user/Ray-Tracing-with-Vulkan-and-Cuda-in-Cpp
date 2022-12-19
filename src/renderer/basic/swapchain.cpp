#include "swapchain.hpp"

#include "logical_device.hpp"
#include "surface.hpp"

namespace neko {

Swapchain::Swapchain(const Settings &crSettings, const Surface &crSurface,
                     const Device &crDevice) {
  /* Vulkan 1.3.231 - A Specification, pg 2235 */

  VkSurfaceCapabilitiesKHR surfaceCapabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(crDevice.physical(), *crSurface,
                                            &surfaceCapabilities);

  uint32_t surfaceFormatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(crDevice.physical(), *crSurface,
                                       &surfaceFormatCount, nullptr);
  std::vector<VkSurfaceFormatKHR> surfaceFormats{surfaceFormatCount};
  vkGetPhysicalDeviceSurfaceFormatsKHR(crDevice.physical(), *crSurface,
                                       &surfaceFormatCount,
                                       surfaceFormats.data());

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(crDevice.physical(), *crSurface,
                                            &presentModeCount, nullptr);
  std::vector<VkPresentModeKHR> presentModes{presentModeCount};
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      crDevice.physical(), *crSurface, &presentModeCount, presentModes.data());

  VkSwapchainCreateInfoKHR swapchainInfo{};
  swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainInfo.pNext = nullptr;

  /* Images of the swapchain can be used to create a VkImageView with a
  different format than what the swapchain was create with. */
  /* If flags contains VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR then the
  pNext chain must include a VkImageFormatListCreateInfo structure with a
  viewFormatCount greater than zero and pViewFormats must have an element
  equal to imageFormat */
  // swapchainInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;

  swapchainInfo.surface = *crSurface;

  /* plus 1 to prevent waiting on the driver to complete internal operations
  before another image is accquired */
  uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

  /* minImageCount <= surfaceCapabilities.maxImageCount, and
  surfaceCapabilities.maxImageCount might be less than
  surfaceCapabilities.minImageCount */
  if (surfaceCapabilities.maxImageCount > 0 &&
      minImageCount > surfaceCapabilities.maxImageCount) {
    minImageCount = surfaceCapabilities.maxImageCount;
  }
  swapchainInfo.minImageCount = minImageCount;

  /* imageFormat and imageColorSpace must match the surfaceFormats.format and
  surfaceFormats.colorSpace members, respectively */
  if (surfaceFormats.empty()) {
    throw std::runtime_error("No surface format available.");
  }
  swapchainInfo.imageFormat = surfaceFormats[0].format;
  swapchainInfo.imageColorSpace = surfaceFormats[0].colorSpace;
  for (const auto &format : surfaceFormats) {
    if (format.format == VK_FORMAT_R8G8B8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      swapchainInfo.imageFormat = format.format;
      swapchainInfo.imageColorSpace = format.colorSpace;
      break;
    }
  }

  /* imageExtent is the size (in pixels) of the swapchain
  image(s). The behavior is platform-dependent if the image
  extent does not match the surface’s currentExtent as returned
  by vkGetPhysicalDeviceSurfaceCapabilitiesKHR. */
  uint32_t imageExtentWidth = surfaceCapabilities.currentExtent.width;
  uint32_t imageExtentHeight = surfaceCapabilities.currentExtent.height;

  /* {0, 0} <= minImageExtent <= imageExtent <= maxImageExtent */
  if (imageExtentWidth > surfaceCapabilities.maxImageExtent.width) {
    imageExtentWidth = surfaceCapabilities.maxImageExtent.width;
  }
  if (imageExtentHeight > surfaceCapabilities.maxImageExtent.height) {
    imageExtentHeight = surfaceCapabilities.maxImageExtent.height;
  }
  swapchainInfo.imageExtent = {imageExtentWidth, imageExtentHeight};

  /* imageArrayLayers is the number of views in a multiview/stereo surface.
  For non-stereoscopic-3D applications, this value is 1. */
  /* 0 < imageArrayLayers <= maxImageArrayLayers */
  swapchainInfo.imageArrayLayers = 1;

  /* {presentMode} determines how incoming present requests will be processed
  and queued internally. */
  /* enum {VkPresentModeKHR}, pg 2219 */
  /* {VK_PRESENT_MODE_FIFO_KHR} is guaranteed to be available */
  swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  auto preferredPresentMode =
      static_cast<VkPresentModeKHR>(crSettings.graphics.presentMode);
  for (const auto &presentMode : presentModes) {
    if (presentMode == preferredPresentMode) {
      swapchainInfo.presentMode = preferredPresentMode;
      break;
    }
  }

  /* {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT} specifies that the image can be
  used to create a {VkImageView} suitable for use as a color or resolve
  attachment in a {VkFramebuffer}, pg 898 */
  swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  /* {VK_SHARING_MODE_EXCLUSIVE} requires some works to be done with ownership
   */
  swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainInfo.queueFamilyIndexCount = 0;
  swapchainInfo.pQueueFamilyIndices = nullptr;

  /* image content is presented without being transformed */
  if (surfaceCapabilities.supportedTransforms &
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
    swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR not supported.");
  }

  if (surfaceCapabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR not supported.");
  }

  swapchainInfo.clipped = VK_TRUE;
  swapchainInfo.pNext = nullptr;
  swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*crDevice, &swapchainInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create swapchain");
  }
  mpcDevice = &crDevice;
  mFormat = swapchainInfo.imageFormat;
  mExtent = swapchainInfo.imageExtent;
}

Swapchain::Swapchain(Swapchain &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mSwapchain{std::exchange(
                                               rhs.mSwapchain, VK_NULL_HANDLE)},
      mFormat{std::move(rhs.mFormat)}, mExtent{std::move(rhs.mExtent)} {}

Swapchain &Swapchain::operator=(Swapchain &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mSwapchain = std::exchange(rhs.mSwapchain, VK_NULL_HANDLE);
  mFormat = std::move(rhs.mFormat);
  mExtent = std::move(rhs.mExtent);
  return *this;
}

void Swapchain::release() noexcept {
  if (mSwapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(**mpcDevice, mSwapchain, nullptr);
    mSwapchain = VK_NULL_HANDLE;
  }
}

} /* namespace neko */