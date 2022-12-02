#include <vulkan_app.hpp>

#include <iostream>

int protected_main() {
  rtvc::VulkanApp app{};
  vk::InstanceCreateInfo instanceInfo{
      .flags{vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR},
  };
  std::cout << vk::to_string(instanceInfo.flags) << '\n';
  return EXIT_SUCCESS;
}

int main() {
  try {
    return protected_main();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Uncaught exception.\n";
  }
  return EXIT_FAILURE;
}