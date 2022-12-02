#ifndef RTVC_INSTANCE_HPP
#define RTVC_INSTANCE_HPP

#include <vkw.hpp>

#include <filesystem>
#include <iostream>

namespace rtvc {

inline const std::string absoluteDirectory =
    std::filesystem::current_path().string();

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData);

class GLFW : public vkw::GLFW {};

class Window : public vkw::Window {};

class Instance : public vkw::Instance {
public:
  using vkw::Instance::Instance;

  Instance() { _initialize(); }

private:
  void _initialize();
};

class DebugMessenger : public vkw::DebugMessenger {
public:
  using vkw::DebugMessenger::DebugMessenger;

  DebugMessenger(const Instance &instance)
      : vkw::DebugMessenger{instance.ref(), &instance.debugInfo()} {}
};

class Surface : public vkw::Surface {
public:
  using vkw::Surface::Surface;

  Surface(const Instance &instance, const vkw::Window &window)
      : vkw::Surface{instance.ref(), window.ref()} {}
};

// void VulkanApp::writeInfo(std::string filePath) {
//   std::fstream fs;
//   fs.open(filePath);
//   if (!fs.is_open()) {
//     throw std::runtime_error("Failed to open " + filePath + '\n');
//   }

//   fs << "1. Instance extensions:\n";
//   fs << "\t1.1 Available:\n";
//   auto availableInstanceExtensions =
//   vkh::getAvailableInstanceExtensionList(); for (const auto &extension :
//   availableInstanceExtensions) {
//     fs << "\t\t" << extension.extensionName << '\n';
//   }
//   fs << "\t1.2 Required:\n";
//   for (const auto &extensionName : instanceExtensions) {
//     fs << "\t\t" << extensionName << '\n';
//   }
//   fs << '\n';

//   fs << "2. Instance layers:\n";
//   fs << "\t2.1 Available:\n";
//   auto availableInstanceLayers = vkh::getAvailableInstanceLayerList();
//   for (const auto &layer : availableInstanceLayers) {
//     fs << "\t\t" << layer.layerName << '\n';
//   }
//   fs << "\t2.2 Required:\n";
//   for (const auto &layer : instanceLayers) {
//     fs << "\t\t" << layer << '\n';
//   }
//   fs << '\n';

//   fs << "3. Physical devices:\n";
//   auto physicalDeviceList = vkh::getPhysicalDeviceList(instance.ref());
//   auto selectedDeviceProperties =
//       vkh::getPhysicalDevicePropertyList(device.physical());
//   size_t index = 1;
//   size_t selectedDeviceIndex = 0;
//   for (const auto &physDev : physicalDeviceList) {
//     std::string selectStr = "";

//     VkPhysicalDeviceVulkan11Properties vk11Props{};
//     vk11Props.sType =
//     VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES; vk11Props.pNext
//     = nullptr;

//     VkPhysicalDeviceVulkan12Properties vk12Props{};
//     vk12Props.sType =
//     VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES; vk12Props.pNext
//     = &vk11Props;

//     VkPhysicalDeviceVulkan13Properties vk13Props{};
//     vk13Props.sType =
//     VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES; vk13Props.pNext
//     = &vk12Props;

//     auto deviceProperties =
//         vkh::getPhysicalDevicePropertyList(physDev, &vk13Props);

//     if (deviceProperties.properties.deviceID ==
//         selectedDeviceProperties.deviceID) {
//       selectedDeviceIndex = index;
//     }
//     if (selectedDeviceIndex == index) {
//       selectStr = " (selected)";
//     }
//     fs << "\t3." << index << " Device " << index << selectStr << ":\n";
//     fs << "\t\t3." << index << ".1 Device properties:\n";
//     fs << "\t\t\tDevice ID: " << deviceProperties.properties.deviceID <<
//     '\n'; fs << "\t\t\tDevice name: " <<
//     deviceProperties.properties.deviceName
//        << '\n';
//     fs << "\t\t\tDevice type: ";
//     switch (deviceProperties.properties.deviceType) {
//     case 1:
//       fs << "Integrated GPU\n";
//       break;
//     case 2:
//       fs << "Discrete GPU\n";
//       break;
//     case 3:
//       fs << "Virtual GPU\n";
//       break;
//     case 4:
//       fs << "CPU\n";
//       break;
//     default:
//       fs << "Other\n";
//       break;
//     }
//     fs << "\t\t\tDriver ID: " << vk12Props.driverID << '\n';
//     fs << "\t\t\tDriver name: " << vk12Props.driverName << '\n';
//     fs << "\t\t\tDriver version: " << vk12Props.driverInfo << '\n';
//     // fs << "\t\t\tDriver version: " << vk13Props. << '\n';
//     fs << "\t\t\tMaximum memory allocation size: "
//        << vk11Props.maxMemoryAllocationSize << '\n';
//     fs << "\t\t3." << index << ".2 Available device extensions:\n";
//     auto availableDeviceExtensions =
//         vkh::getAvailableDeviceExtensionList(physDev);
//     for (const auto &extension : availableDeviceExtensions) {
//       fs << "\t\t\t" << extension.extensionName << '\n';
//     }
//     if (selectedDeviceIndex == index) {
//       fs << "\t\t3." << index << ".3 Required device extensions:\n";
//       for (const auto &extension : deviceExtensions) {
//         fs << "\t\t\t" << extension << '\n';
//       }
//     }
//     fs << '\n';
//     ++index;
//   }
//   fs << '\n';
// }

} /* namespace rtvc */
#endif /* RTVC_INSTANCE_HPP */