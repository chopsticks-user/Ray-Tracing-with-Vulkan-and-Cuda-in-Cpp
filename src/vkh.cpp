#include "vkh.hpp"

void VulkanApp::createWindow() {
  glfwInit();
  window = vkh::createWindow(800, 600);
}

void VulkanApp::createInstance() {
  auto appInfo = vkh::makeApplicationInfo();
  auto extensions = vkh::getRequiredInstanceExtensionNameList(true);
  auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};
  vkh::populateDebugMessengerCreateInfo(debugCreateInfo);
  auto createInfo = vkh::makeInstanceCreateInfo(
      &appInfo, static_cast<uint32_t>(extensions.size()), extensions.data(),
      static_cast<uint32_t>(layers.size()), layers.data(),
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo);
  instance = vkh::createInstance(&createInfo);
}

void VulkanApp::createDebugMessenger() {
  debugMessenger = vkh::createDebugMessenger(instance);
}

void VulkanApp::createSurface() {
  surface = vkh::createSurface(instance, window);
}

std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
VulkanApp::selectQueueFamily(VkPhysicalDevice physDev) {
  auto graphicsFamilies = vkh::getGraphicsQueueFamilyPropertyList(physDev);
  auto computeFamilies = vkh::getComputeQueueFamilyPropertyList(physDev);
  auto presentFamilies =
      vkh::getPresentQueueFamilyPropertyList(physDev, surface);

  /* Find a queue that is capable of handling graphics, computing, and
  supporting presentation to the surface */
  for (size_t i = 0; i < graphicsFamilies.size(); ++i) {
    if ((graphicsFamilies[i].has_value() && computeFamilies[i].has_value()) &&
        presentFamilies[i].has_value()) {
      return {{static_cast<uint32_t>(i), graphicsFamilies[i].value()}};
    }
  }
  return {};
}

void VulkanApp::createDevice() {
  /* Select a suitable physical device and one of its queue families */
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
  VkPhysicalDevice selectedPhysDev = VK_NULL_HANDLE;
  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
      selectedQueueFamily;
  for (const auto &physDev : physicalDeviceList) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physDev);
    /* Only selected a discrete GPU that has a queue family supporting
    graphics, computing, and presentation commands */
    /* When the application creates a swapchain, the selected physical device
    must support VK_KHR_swapchain */
    /* Also, the selected physical device must be compatible with the swapchain
     that will be created. */
    if ((physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
        checkDeviceExtensionSupport(physDev) &&
        checkDeviceSwapchainSupport(physDev)) {
      if (auto returnedQueueFamily = selectQueueFamily(physDev);
          returnedQueueFamily.has_value()) {
        selectedQueueFamily = returnedQueueFamily;
        selectedPhysDev = physDev;
      }
    }
  }
  if (selectedPhysDev == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to select a physical device.");
  }

  /* Set up the selected queue family's creation info */
  uint32_t selectedIndex = selectedQueueFamily.value().first;
  VkDeviceQueueCreateInfo queueCreateInfo;
  const float queuePriority = 1.0f;
  queueCreateInfo = vkh::makeDeviceQueueCreateInfo(
      selectedQueueFamily.value().first, 1, &queuePriority);

  /* Create the logical device */
  VkPhysicalDeviceFeatures deviceFeatures{};
  auto deviceCreateInfo = vkh::makeDeviceCreateInfo(
      1, &queueCreateInfo, static_cast<uint32_t>(deviceExtensions.size()),
      deviceExtensions.data(), &deviceFeatures,
      static_cast<uint32_t>(layers.size()), layers.data());
  device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);

  /* Get a queue handle */
  vkGetDeviceQueue(device, selectedIndex, 0, &queue);

  /* Store the selected device for later uses */
  this->physicalDevice = selectedPhysDev;
}

bool VulkanApp::checkDeviceExtensionSupport(VkPhysicalDevice physDev) {
  auto availableExtensionsList = vkh::getAvailableDeviceExtensionList(physDev);
  std::map<std::string, uint32_t> helper;
  for (const auto &availableExtension : availableExtensionsList) {
    helper[availableExtension.extensionName]++;
  }
  for (const auto &deviceExtension : deviceExtensions) {
    helper[deviceExtension]++;
    if (helper[deviceExtension] < 2) {
      return false;
    }
  }
  return true;
}

bool VulkanApp::checkDeviceSwapchainSupport(
    [[maybe_unused]] VkPhysicalDevice physDev) {
  return true;
}

VkSwapchainCreateInfoKHR VulkanApp::populateSwapchainCreateInfo() {
  /* Vulkan 1.3.231 - A Specification, pg 2235 */

  auto surfaceCapabilities =
      vkh::getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface);
  auto surfaceFormats =
      vkh::getPhysicalDeviceSurfaceFormatList(physicalDevice, surface);
  auto surfacePresentModes =
      vkh::getPhysicalDeviceSurfacePresentModeList(physicalDevice, surface);

  VkSwapchainCreateInfoKHR swapchainCreateInfo{};
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = nullptr;

  /* Images of the swapchain can be used to create a VkImageView with a
  different format than what the swapchain was create with. */
  /* {VK_KHR_swapchain_mutable_format} is not available */
  // swapchainCreateInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;

  swapchainCreateInfo.surface = surface;

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
  swapchainCreateInfo.minImageCount = minImageCount;

  /* imageFormat and imageColorSpace must match the surfaceFormats.format and
  surfaceFormats.colorSpace members, respectively */
  if (surfaceFormats.empty()) {
    throw std::runtime_error("No surface format available.");
  }
  swapchainCreateInfo.imageFormat = surfaceFormats[0].format;
  swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace;
  for (const auto &format : surfaceFormats) {
    if (format.format == VK_FORMAT_R8G8B8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      swapchainCreateInfo.imageFormat = format.format;
      swapchainCreateInfo.imageColorSpace = format.colorSpace;
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
  swapchainCreateInfo.imageExtent = {imageExtentWidth, imageExtentHeight};

  /* imageArrayLayers is the number of views in a multiview/stereo surface. For
  non-stereoscopic-3D applications, this value is 1. */
  /* 0 < imageArrayLayers <= maxImageArrayLayers */
  swapchainCreateInfo.imageArrayLayers = 1;

  /* {presentMode} determines how incoming present requests will be processed
  and queued internally. */
  /* enum {VkPresentModeKHR}, pg 2219 */
  /* {VK_PRESENT_MODE_FIFO_KHR} is guaranteed to be available */
  swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  auto preferredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  for (const auto &presentMode : surfacePresentModes) {
    if (presentMode == preferredPresentMode) {
      swapchainCreateInfo.presentMode = preferredPresentMode;
      break;
    }
  }

  /* {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT} specifies that the image can be used
  to create a {VkImageView} suitable for use as a color or resolve attachment in
  a {VkFramebuffer}, pg 898 */
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  /* {VK_SHARING_MODE_EXCLUSIVE} requires some works to be done with ownership
   */
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;

  /* image content is presented without being transformed */
  if (surfaceCapabilities.supportedTransforms &
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR not supported.");
  }

  if (surfaceCapabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else {
    throw std::runtime_error(
        "VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR not supported.");
  }

  swapchainCreateInfo.clipped = VK_TRUE;

  swapchainCreateInfo.pNext = nullptr;
  swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

  return swapchainCreateInfo;
}

void VulkanApp::createSwapchain() {
  auto swapchainCreateInfo = populateSwapchainCreateInfo();
  swapchain = vkh::createSwapchain(device, &swapchainCreateInfo);
}

VulkanApp::VulkanApp() {
  createWindow();
  createInstance();
  createDebugMessenger();
  createSurface();
  createDevice();
  createSwapchain();
}

VulkanApp::~VulkanApp() {
  vkh::destroySwapchain(device, swapchain);
  vkh::destroyDevice(device);
  vkh::destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  vkh::destroySurface(instance, surface);
  vkh::destroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void VulkanApp::run() {
  vkh::Timer time_total;
  const double sec_to_mics = 1'000'000.0;
  while (!glfwWindowShouldClose(window)) {
    vkh::Timer time_circle;

    glfwPollEvents();

    double current = static_cast<double>(time_total.current());
    if (current >= sec_to_mics) {
      std::cout << sec_to_mics / static_cast<double>(time_circle.current())
                << " FPS\n";
      time_total.reset();
    }
  }
  vkDeviceWaitIdle(device);
}