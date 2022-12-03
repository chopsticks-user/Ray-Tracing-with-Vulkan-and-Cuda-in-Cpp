#include <vulkan_app.hpp>

#include <iostream>

int protected_main() {
  auto app = new rtvc::VulkanApp{};
  app->render();
  delete app;
  return EXIT_SUCCESS;
}

int main() {
  try {
    return protected_main();
  } catch (vk::SystemError &e) {
    std::cerr << e.what() << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}