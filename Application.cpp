#include <vulkan_app.hpp>

int protected_main() {
  VulkanApp app;
  app.run();
  app.writeInfo("info.log");
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