// #include "physical.hpp"

// #include "renderer/basic/instance/instance.hpp"

// namespace neko {

// PhysicalDevices::PhysicalDevices(const Instance &crInstance) {
//   uint32_t physicalDeviceCount;
//   VkResult result;
//   if (result = vkEnumeratePhysicalDevices(*crInstance, &physicalDeviceCount,
//                                           nullptr);
//       result != VK_SUCCESS && result != VK_INCOMPLETE) {
//     throw std::runtime_error("Failed to enumerate physical devices.");
//   }
//   if (physicalDeviceCount == 0) {
//     throw std::runtime_error("Failed to find any physical device.");
//   }
//   mPhysicalDevices.resize(physicalDeviceCount);
//   if (result = vkEnumeratePhysicalDevices(*crInstance, &physicalDeviceCount,
//                                           mPhysicalDevices.data());
//       result != VK_SUCCESS && result != VK_INCOMPLETE) {
//     throw std::runtime_error("Failed to enumerate physical devices.");
//   }
//   incomplete = (result == VK_INCOMPLETE);
// }

// std::vector<VkPhysicalDeviceProperties>
// PhysicalDevices::getProperties(VkPhysicalDeviceProperties properties) const {
//   std::vector<VkPhysicalDeviceProperties> propertiesList{
//       mPhysicalDevices.size(), properties};
//   for (size_t iDevice = 0; iDevice < mPhysicalDevices.size(); ++iDevice) {
//     vkGetPhysicalDeviceProperties(mPhysicalDevices[iDevice],
//                                   &propertiesList[iDevice]);
//   }
//   return propertiesList;
// }

// std::vector<VkPhysicalDeviceProperties2>
// PhysicalDevices::getProperties(VkPhysicalDeviceProperties2 properties) const
// {
//   std::vector<VkPhysicalDeviceProperties2> propertiesList{
//       mPhysicalDevices.size(), properties};
//   for (size_t iDevice = 0; iDevice < mPhysicalDevices.size(); ++iDevice) {
//     vkGetPhysicalDeviceProperties2(mPhysicalDevices[iDevice],
//                                    &propertiesList[iDevice]);
//   }
//   return propertiesList;
// }

// } /* namespace neko */