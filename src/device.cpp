#include "device.hpp"

namespace rtvc {

Device::Device(const Instance &instance, const vkw::Surface &surface) {
  _initialize(instance.ref(), surface.ref());
}

void Device::_initialize(VkInstance instance, VkSurfaceKHR surface) {
  static const std::vector<const char *> requiredExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME};

  static const std::vector<const char *> instanceLayers = {
      "VK_LAYER_KHRONOS_validation", "VK_LAYER_MANGOHUD_overlay"};

  /* Get a list of available physical devices */
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
  if (physicalDeviceCount == 0) {
    throw std::runtime_error(
        "Failed to find any physical device with Vulkan support.");
  }
  std::vector<VkPhysicalDevice> physicalDevices{physicalDeviceCount};
  if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
                                 physicalDevices.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get physical devices list.");
  };

  /* Select a suitable physical device and one of its queue families */
  VkPhysicalDevice selectedPhysicalDeivce = VK_NULL_HANDLE;
  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
      selectedQueueFamily;
  for (const auto &physicalDevice : physicalDevices) {
    /* Only selected a discrete GPU that has a queue family supporting
    graphics, computing, and presentation commands */
    /* When the application creates a swapchain, the selected physical device
    must support VK_KHR_swapchain */
    /* Also, the selected physical device must be compatible with the
     swapchain that will be created. */
    if (_checkProperties(physicalDevice) &&
        _checkExtensionSupport(physicalDevice, requiredExtensions) &&
        _checkSwapchainSupport(physicalDevice) &&
        _checkFeatureSupport(physicalDevice)) {
      if (auto returnedQueueFamily =
              _selectQueueFamily(surface, physicalDevice);
          returnedQueueFamily.has_value()) {
        selectedQueueFamily = returnedQueueFamily;
        selectedPhysicalDeivce = physicalDevice;
      }
    }
  }
  if (selectedPhysicalDeivce == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to select a physical device.");
  }

  /* Set up the selected queue family's creation info */
  uint32_t selectedIndex = selectedQueueFamily.value().first;
  const float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueInfo{};
  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueInfo.pNext = nullptr;
  queueInfo.queueFamilyIndex = selectedIndex;
  queueInfo.queueCount = 1;
  queueInfo.pQueuePriorities = &queuePriority;

  /* Create the logical device */
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pNext = nullptr;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pQueueCreateInfos = &queueInfo;
  deviceCreateInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  deviceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(instanceLayers.size());
  deviceCreateInfo.ppEnabledLayerNames = instanceLayers.data();

  vkCreateDevice(selectedPhysicalDeivce, &deviceCreateInfo, nullptr, &_device);
  _physicalDevice = selectedPhysicalDeivce;
  vkGetDeviceQueue(_device, selectedIndex, 0, &_queue.ref);
  _queue.familyIndex = selectedIndex;
  _isOwner = true;
}

bool Device::_checkProperties(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);
  return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

bool Device::_checkExtensionSupport(
    VkPhysicalDevice physicalDevice,
    const std::vector<const char *> &requiredExtensions) {
  /* Get available device extensions */
  uint32_t propertyCount;
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &propertyCount, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }
  std::vector<VkExtensionProperties> availableExtensions{propertyCount};
  if (vkEnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &propertyCount,
          availableExtensions.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to get device extension list.");
  }

  /* Check if all required extension are supported */
  std::map<std::string, uint32_t> helper;
  for (const auto &availableExtension : availableExtensions) {
    helper[availableExtension.extensionName]++;
    //  std::cout << availableExtension.extensionName << '\n';
  }
  for (const auto &requiredExtension : requiredExtensions) {
    helper[requiredExtension]++;
    if (helper[requiredExtension] < 2) {
      return false;
    }
  }
  return true;
}

bool Device::_checkSwapchainSupport(
    [[maybe_unused]] VkPhysicalDevice physicalDevice) {
  return true;
}

bool Device::_checkFeatureSupport(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceFeatures supportedFeatures{};
  vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
  return supportedFeatures.samplerAnisotropy;
}

std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
Device::_selectQueueFamily(VkSurfaceKHR surface,
                           VkPhysicalDevice physicalDevice) {
  /* Get queue families' properties */
  uint32_t queueFamilyPropertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                           &queueFamilyPropertyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilyProperties{
      queueFamilyPropertyCount};
  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

  /* Find a queue that is capable of handling graphics, computing, and
  supporting presentation to the surface */
  /* any queue family with {VK_QUEUE_GRAPHICS_BIT} or {VK_QUEUE_COMPUTE_BIT}
  capabilities already implicitly support {VK_QUEUE_TRANSFER_BIT} operations */
  uint32_t index = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if ((queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
        (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
      VkBool32 presentSupported = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface,
                                           &presentSupported);
      return {{index, queueFamilyProperty}};
    }
  }
  return {};
}

} /* namespace rtvc */