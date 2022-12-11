#include <neko.hpp>

#include <iostream>

int protected_main() {
  auto timer = neko::ScopedTimer{neko::TimeUnit::milliseconds};
  auto engine = std::make_unique<neko::Engine>();
  timer.invoke([](float x) { std::cout << "Load time: " << x << " ms\n"; });

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