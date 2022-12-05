#include "device.hpp"

#include <map>
#include <optional>

namespace rtvc {

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

} /* namespace rtvc */