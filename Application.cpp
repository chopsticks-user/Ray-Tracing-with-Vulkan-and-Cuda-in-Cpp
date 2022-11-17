#include <vkh.hpp>

#include <iostream>
#include <typeinfo>

int protected_main() {
  VulkanApp app;
  app.run();
  // std::cout << 2 & 1 << '\n';
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