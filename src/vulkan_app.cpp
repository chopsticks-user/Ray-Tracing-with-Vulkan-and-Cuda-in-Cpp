#include "vulkan_app.hpp"

namespace rtvc {

vk::raii::Instance VulkanApp::makeInstance() {
  vk::ApplicationInfo applicationInfo{
      .pApplicationName{"Vulkan Application"},
      .applicationVersion{VK_MAKE_VERSION(1, 0, 0)},
      .apiVersion{VK_API_VERSION_1_1},
  };

  vk::InstanceCreateInfo instanceInfo{
      .pApplicationInfo{&applicationInfo},
  };

  vk::raii::Instance instance{
      _context,
      instanceInfo,
  };
  return instance;
}

vk::raii::DebugUtilsMessengerEXT makeDebugMessenger() {
  //   vk::raii::DebugUtilsMessengerEXT debugMessenger{

  //   };
}

} /* namespace rtvc */
