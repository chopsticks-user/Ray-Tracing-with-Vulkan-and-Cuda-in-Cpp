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

std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
VulkanApp::selectQueueFamily(VkPhysicalDevice physicalDevice) {
  auto graphicsFamilies =
      vkh::getGraphicsQueueFamilyPropertyList(physicalDevice);
  auto computeFamilies = vkh::getComputeQueueFamilyPropertyList(physicalDevice);
  auto presentFamilies =
      vkh::getPresentQueueFamilyPropertyList(physicalDevice, surface);

  /* Find a queue that is capable of handling graphics, computing, and
  presentation commands */
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
  for (const auto &physicalDevice : physicalDeviceList) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physicalDevice);
    /* Only selected a discrete GPU that has a queue family supporting
    graphics, computing, and presentation commands */
    if (physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      if (auto returnedQueueFamily = selectQueueFamily(physicalDevice);
          returnedQueueFamily.has_value()) {
        selectedQueueFamily = returnedQueueFamily;
        selectedPhysDev = physicalDevice;
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
      1, &queueCreateInfo, &deviceFeatures,
      static_cast<uint32_t>(layers.size()), layers.data());
  device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);

  /* Get a queue handle */
  vkGetDeviceQueue(device, selectedIndex, 0, &queue);
}

void VulkanApp::createSurface() {
  surface = vkh::createSurface(instance, window);
}

VulkanApp::VulkanApp() {
  createWindow();
  createInstance();
  createDebugMessenger();
  createSurface();
  createDevice();
}

VulkanApp::~VulkanApp() {
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

    if (time_total.current() >= sec_to_mics) {
      std::cout << sec_to_mics / time_circle.current() << " FPS\n";
      time_total.reset();
    }
  }

  vkDeviceWaitIdle(device);
}