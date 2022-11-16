#include <vkh.hpp>

#include <iostream>
#include <typeinfo>

int protected_main() {
  glfwInit();

  auto appInfo = vkh::makeApplicationInfo();
  auto createInfo = vkh::makeInstanceCreateInfo(&appInfo);
  auto instance = vkh::createInstance(&createInfo);
  auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);

  auto layerList = vkh::getAvailableInstanceLayerList();
  for (const auto &layer : layerList) {
    std::cout << layer.layerName << '\n';
  }

  

  vkh::destroyInstance(instance, nullptr);

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