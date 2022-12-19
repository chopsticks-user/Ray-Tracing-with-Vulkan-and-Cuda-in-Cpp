#include <filesystem>
#include <iostream>
#include <neko.hpp>

static const std::string settingsFilePath =
    std::filesystem::current_path().string() + "/data/configs/settings.json";

static int protected_main([[maybe_unused]] int argc,
                          [[maybe_unused]] char **argv) {
  auto engine = std::make_unique<neko::Engine>(settingsFilePath);
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