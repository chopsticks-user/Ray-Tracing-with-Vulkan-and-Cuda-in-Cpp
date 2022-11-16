#include "src/VulkanApp.hpp"
#include <Renderer.hpp>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  // rtvc::RenderApplication app;
  // app.run();
  glfwInit();

  auto appInfo = vkh::makeApplicationInfo();
  auto createInfo = vkh::makeInstanceCreateInfo(&appInfo);
  auto instance = vkh::createInstance(&createInfo);

  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
  std::cout << physicalDeviceList.size() << '\n';

  for (const auto &device : physicalDeviceList) {
    VkPhysicalDeviceVulkan12Properties vk12Props{};
    vk12Props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
    VkPhysicalDeviceProperties2 properties{};
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    properties.pNext = &vk12Props;
    vkGetPhysicalDeviceProperties2(device, &properties);
    std::cout << vk12Props.driverName << '\n';
    std::cout << properties.properties.vendorID << '\n';
  }

  vkDestroyInstance(instance, nullptr);

  // auto window = vkh::createWindow(800, 600);
  // while (!glfwWindowShouldClose(window)) {
  //   glfwPollEvents();
  // }

  glfwTerminate();
  return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Uncaught exception.\n";
  }
  return EXIT_FAILURE;
}