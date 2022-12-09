#include <neko.hpp>

#include <chrono>
#include <iostream>
#include <thread>

int protected_main() {
  auto start = std::chrono::steady_clock::now();
  auto engine = std::make_unique<neko::Engine>();
  auto end = std::chrono::steady_clock::now();
  std::cout << "Load time: "
            << static_cast<float>(
                   std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                         start)
                       .count()) /
                   1000.0f
            << " ms\n";
  engine->start();
  std::cout << sizeof(neko::Engine) << '\n';
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