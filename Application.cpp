#include <vulkan_app.hpp>

int protected_main() {
  glfwInit();
  VulkanApp app;
  app.run();
  app.writeInfo(vkh::absoluteDirectory + "/info.log");
  glfwTerminate();
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