#include <vulkan_app.hpp>

int protected_main() {
  auto app = new VulkanApp{};
  app->run();
  app->writeInfo(vkh::absoluteDirectory + "/info.log");
  delete app;
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