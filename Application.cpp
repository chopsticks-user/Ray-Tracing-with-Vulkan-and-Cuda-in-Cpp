#include <iostream>
#include <neko.hpp>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  TIMER_START(t1);
  auto engine = std::make_unique<neko::Engine>();
  TIMER_INVOKE(t1, "Engine's load time");
  engine->start();
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}