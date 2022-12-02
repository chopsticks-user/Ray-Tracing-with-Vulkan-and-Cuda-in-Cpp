#include <iostream>
#include <vulkan/vulkan.hpp>

int protected_main() {
  vk::Instance instance{};
  instance.destroy();
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