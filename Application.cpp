#include <Renderer.hpp>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  rtvc::RenderApplication app;
  app.run();

  return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Uncaught exception.\n";
  }
  return EXIT_FAILURE;
}