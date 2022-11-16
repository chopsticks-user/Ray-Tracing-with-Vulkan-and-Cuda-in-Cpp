#include "vkh.hpp"

void VulkanApp::createWindow() {
  glfwInit();
  window = vkh::createWindow(800, 600);
}

void VulkanApp::createInstance() {
  auto appInfo = vkh::makeApplicationInfo();
  const std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};
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

void VulkanApp::createDevice() {

  /* Select a suitable physical device */
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
  VkPhysicalDevice selectedPhysDev = VK_NULL_HANDLE;
  for (const auto &physicalDevice : physicalDeviceList) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physicalDevice);
    if (physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      selectedPhysDev = physicalDevice;
      break;
    }
  }
  if (selectedPhysDev == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to select a physical device.");
  }

  /* Setup the selected device's createInfo */
  auto deviceCreateInfo = vkh::makeDeviceCreateInfo(0, nullptr);

  /* Create the logical device */
  device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);
}

VulkanApp::VulkanApp() {
  createWindow();
  createInstance();
  createDebugMessenger();
  createDevice();
}

VulkanApp::~VulkanApp() {
  vkh::destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  vkh::destroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}