#include "vulkan_app.hpp"

#include <map>
#include <optional>

namespace rtvc {

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData) {
  if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    std::cerr << '\n' << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

bool validationLayerSupported() {
  auto supportedLayers = vk::enumerateInstanceLayerProperties();
  for (const auto &supportedLayer : supportedLayers) {
    if (supportedLayer.layerName == validationLayerName) {
      return true;
    }
  }
  return false;
}

vk::raii::Instance makeInstance(const vk::raii::Context &context) {
  vk::ApplicationInfo applicationInfo{
      .pApplicationName{"Vulkan Application"},
      .applicationVersion{VK_MAKE_VERSION(1, 0, 0)},
      .apiVersion{VK_API_VERSION_1_0},
  };

  // Required extensions
  uint32_t requiredExtensionCount = 0;
  auto ppRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char *> requiredExtensions{requiredExtensionCount};
  for (size_t i = 0; i < requiredExtensionCount; ++i) {
    requiredExtensions[i] = ppRequiredExtensions[i];
  }

  // Requested layers
  std::vector<const char *> requestedLayers = {
      mangohudLayerName.data(),
  };

  // Enable validation layers if the application is running in debug mode
  vk::DebugUtilsMessengerCreateInfoEXT *pDebugInfo = nullptr;
  vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
  if constexpr (debugMode) {
    if (validationLayerSupported()) {
      requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      requestedLayers.push_back(validationLayerName.data());

      debugInfo = makeDebugMessengerInfo();
      pDebugInfo = &debugInfo;
    }
  }

  vk::InstanceCreateInfo instanceInfo{
      .pNext{pDebugInfo},
      .pApplicationInfo{&applicationInfo},
      .enabledLayerCount{static_cast<uint32_t>(requestedLayers.size())},
      .ppEnabledLayerNames{requestedLayers.data()},
      .enabledExtensionCount{static_cast<uint32_t>(requiredExtensions.size())},
      .ppEnabledExtensionNames{requiredExtensions.data()},
  };
  return {context, instanceInfo};
}

[[nodiscard]] vk::DebugUtilsMessengerCreateInfoEXT makeDebugMessengerInfo() {
  return {
      .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
      .messageType =
          vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
      .pfnUserCallback = debugCallback,
  };
}

vk::raii::DebugUtilsMessengerEXT
makeDebugMessenger(const vk::raii::Instance &instance) {
  if constexpr (debugMode) {
    return {instance, makeDebugMessengerInfo()};
  }
  return {VK_NULL_HANDLE};
}

vk::raii::SurfaceKHR makeSurface(const vk::raii::Instance &instance,
                                 const Window &window) {
  VkSurfaceKHR surface;
  glfwCreateWindowSurface(static_cast<VkInstance>(*instance), window.ref(),
                          nullptr, &surface);
  return {instance, surface};
}

DeviceWrapper makeLogicalDevice(const vk::raii::Instance &instance,
                                const vk::raii::SurfaceKHR &surface) {
  static const std::vector<const char *> requiredExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
  static const std::vector<const char *> instanceLayers = {
      validationLayerName.data(),
      mangohudLayerName.data(),
  };

  vk::raii::PhysicalDevices physicalDevices{instance};
  for (const auto &physicalDevice : physicalDevices) {
    if (auto properties = physicalDevice.getProperties();
        properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu) {
      continue;
    }

    bool extensionsSupported = true;
    auto supportedExtensions =
        physicalDevice.enumerateDeviceExtensionProperties();
    std::map<std::string, uint32_t> extensionsMap{};
    for (const auto &extension : supportedExtensions) {
      extensionsMap[extension.extensionName]++;
    }
    for (const auto &extension : requiredExtensions) {
      extensionsMap[extension]++;
      if (extensionsMap[extension] < 2) {
        extensionsSupported = false;
      }
    }
    if (!extensionsSupported) {
      continue;
    }

    bool layerSupported = true;
    auto supportedLayers = physicalDevice.enumerateDeviceLayerProperties();
    std::map<std::string, uint32_t> layerMap{};
    for (const auto &layer : supportedLayers) {
      layerMap[layer.layerName]++;
    }
    for (const auto &layer : instanceLayers) {
      layerMap[layer]++;
      if (layerMap[layer] < 2) {
        layerSupported = false;
      }
    }
    if (!layerSupported) {
      continue;
    }

    if (auto features = physicalDevice.getFeatures();
        !features.samplerAnisotropy) {
      continue;
    }

    auto queueFamilies = physicalDevice.getQueueFamilyProperties();
    std::optional<uint32_t> selectedQueueFamilyIndex;
    for (size_t i = 0; i < queueFamilies.size(); ++i) {
      if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
          (queueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute) &&
          (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i),
                                               *surface))) {
        selectedQueueFamilyIndex = static_cast<uint32_t>(i);
      }
    }
    if (!selectedQueueFamilyIndex.has_value()) {
      throw std::runtime_error("Failed to select queue families");
    }

    const float queuePriorities = 1.0f;
    vk::DeviceQueueCreateInfo queueInfo{
        .queueFamilyIndex{selectedQueueFamilyIndex.value()},
        .queueCount{1},
        .pQueuePriorities{&queuePriorities},
    }; // namespace rtvc

    vk::PhysicalDeviceFeatures features{};
    // features.samplerAnisotropy = VK_TRUE;
    // features.sample

    vk::DeviceCreateInfo deviceInfo{
        .queueCreateInfoCount{1},
        .pQueueCreateInfos{&queueInfo},
        .enabledLayerCount{static_cast<uint32_t>(instanceLayers.size())},
        .ppEnabledLayerNames{instanceLayers.data()},
        .enabledExtensionCount{
            static_cast<uint32_t>(requiredExtensions.size())},
        .ppEnabledExtensionNames{requiredExtensions.data()},
        .pEnabledFeatures{&features},
    };

    auto device = physicalDevice.createDevice(deviceInfo);
    auto queue = device.getQueue(selectedQueueFamilyIndex.value(), 0);

    return {
        selectedQueueFamilyIndex.value(),
        queue,
        physicalDevice,
        physicalDevice.createDevice(deviceInfo),
    };
  }
  throw std::runtime_error("Failed to create a logical device");
}

vk::raii::CommandPool makeCommandPool(const DeviceWrapper &device) {
  return {
      device.logical,
      {
          .queueFamilyIndex{device.queueFamilyIndex},
      },
  };
}
} /* namespace rtvc */
