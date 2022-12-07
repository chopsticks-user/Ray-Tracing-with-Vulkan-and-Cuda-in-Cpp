#include <neko.hpp>

#include <iostream>

int protected_main() {
  auto engine = new neko::Engine();
  engine->start();
  delete engine;
  return EXIT_SUCCESS;
}

int main() {
  try {
    return protected_main();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}