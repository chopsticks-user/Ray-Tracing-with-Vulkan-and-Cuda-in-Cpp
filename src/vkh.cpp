#include "vkh.hpp"

void VulkanApp::createWindow() {
  glfwInit();
  this->window = vkh::createWindow(800, 600);
}

void VulkanApp::createInstance() {
  auto appInfo = vkh::makeApplicationInfo();
  auto extensions = vkh::getRequiredInstanceExtensionNameList(true);
  auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{};
  vkh::populateDebugMessengerCreateInfo(debugCreateInfo);
  auto createInfo = vkh::makeInstanceCreateInfo(
      &appInfo, static_cast<uint32_t>(extensions.size()), extensions.data(),
      static_cast<uint32_t>(this->layers.size()), this->layers.data(),
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo);
  this->instance = vkh::createInstance(&createInfo);
}

void VulkanApp::createDebugMessenger() {
  this->debugMessenger = vkh::createDebugMessenger(this->instance);
}

void VulkanApp::createDevice() {

  /* Select a suitable physical device */
  auto physicalDeviceList = vkh::getPhysicalDeviceList(this->instance);
  VkPhysicalDevice selectedPhysDev = VK_NULL_HANDLE;
  QueueFamilyIndices selectedIndices;
  for (const auto &physicalDevice : physicalDeviceList) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physicalDevice);
    if (physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      /* Select queue families */
      QueueFamilyIndices indices;
      auto queueFamilies =
          vkh::getPhysicalDeviceQueuePropertyList(physicalDevice);
      for (std::size_t i = 0; i < queueFamilies.size(); ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
          //   std::cout << "graphics: " << indices.graphicsFamily.value() <<
          //   '\n';
        }
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
          indices.computeFamily = i;
          //   std::cout << "compute: " << indices.computeFamily.value() <<
          //   '\n';
        }
      }
      if (indices.isComplete()) {
        std::cout << physDevPropList.deviceName << '\n';
        selectedPhysDev = physicalDevice;
        selectedIndices.graphicsFamily = indices.graphicsFamily;
        selectedIndices.computeFamily = indices.computeFamily;
        break;
      }
    }
  }
  if (selectedPhysDev == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to select a physical device.");
  }

  /* Setup the selected queue families' createInfos */
  VkDeviceQueueCreateInfo queueCreateInfos[2];
  const float queuePriorities[2] = {1.0f, 0.8f};
  queueCreateInfos[0] = vkh::makeDeviceQueueCreateInfo(
      selectedIndices.graphicsFamily.value(), 1, &queuePriorities[0]);
  queueCreateInfos[1] = vkh::makeDeviceQueueCreateInfo(
      selectedIndices.computeFamily.value(), 1, &queuePriorities[1]);

  /* Setup the selected device's createInfo */

  /* Create the logical device */
  auto deviceCreateInfo = vkh::makeDeviceCreateInfo(
      2, queueCreateInfos, nullptr, static_cast<uint32_t>(this->layers.size()),
      this->layers.data());
  this->device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);
}

VulkanApp::VulkanApp() {
  createWindow();
  createInstance();
  createDebugMessenger();
  createDevice();
}

VulkanApp::~VulkanApp() {
  vkh::destroyDevice(this->device);
  vkh::destroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger,
                                     nullptr);
  vkh::destroyInstance(this->instance, nullptr);
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(this->window)) {
    glfwPollEvents();
  }
}