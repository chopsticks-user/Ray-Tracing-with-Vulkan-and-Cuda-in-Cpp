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

  for (const auto &ext : extensions) {
    std::cout << ext << '\n';
  }
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
    if ((physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
        checkDeviceExtensionSupport(physDev)) {
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

VkSwapchainCreateInfoKHR VulkanApp::populateSwapchainCreateInfoKHR() {
  /* Vulkan 1.3.231 - A Specification, pg 2235 */

  auto surfaceCapabilities =
      vkh::getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface);
  auto surfaceFomats =
      vkh::getPhysicalDeviceSurfaceFormatList(physicalDevice, surface);

  VkSwapchainCreateInfoKHR swapchainCreateInfo{};
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = nullptr;

  /* Images of the swapchain can be used to create a VkImageView with a
   * different format than what the swapchain was create with. */
  swapchainCreateInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;

  swapchainCreateInfo.surface = surface;

  /* plus 1 to prevent waiting on the driver to complete internal opeerations
   * before another image is accquired */
  uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

  /* minImageCount <= surfaceCapabilities.maxImageCount, and
  surfaceCapabilities.maxImageCount might be less than
  surfaceCapabilities.minImageCount */
  if (surfaceCapabilities.maxImageCount > 0 &&
      minImageCount > surfaceCapabilities.maxImageCount) {
    minImageCount = surfaceCapabilities.maxImageCount;
  }
  swapchainCreateInfo.minImageCount = minImageCount;

  /*  */

  // swapchainCreateInfo.imageFormat = ;

  return swapchainCreateInfo;
}

void VulkanApp::createSwapchain() {
  [[maybe_unused]] auto swapchainCreateInfo = populateSwapchainCreateInfoKHR();
  // swapchain = vkh::createSwapchain(device, &swapchainCreateInfo);
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
  // vkh::destroySwapchain(device, swapchain);
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