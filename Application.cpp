#include <vkh.hpp>

#include <iostream>
#include <typeinfo>

int protected_main() {
  glfwInit();

  auto appInfo = vkh::makeApplicationInfo();
  auto createInfo = vkh::makeInstanceCreateInfo(&appInfo);
  auto instance = vkh::createInstance(&createInfo);

  vkDestroyInstance(instance, nullptr);

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