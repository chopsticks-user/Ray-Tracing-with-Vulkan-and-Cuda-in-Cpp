#include <iostream>
#include <neko.hpp>

int protected_main() {
  TIMER_START(t1);
  auto engine = std::make_unique<neko::Engine>();
  TIMER_STOP(t1, "Engine's load time");
  engine->start();
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