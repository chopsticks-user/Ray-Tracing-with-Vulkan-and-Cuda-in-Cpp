#ifndef NEKO_INTERFACE_USER_ENTRY_HPP
#define NEKO_INTERFACE_USER_ENTRY_HPP

#include <filesystem>
#include <iostream>

static const std::string configFilePath =
    std::filesystem::current_path().string() + "/Data/Configs/settings.json";

static const std::string vertexShaderPath =
    std::filesystem::current_path().string() +
    "/Data/Resources/Shaders/Basic.vert.spv";
static const std::string fragmentShaderPath =
    std::filesystem::current_path().string() +
    "/Data/Resources/Shaders/Basic.frag.spv";

extern neko::Engine *initializeEngine(const std::string &configFilePath);

static int protected_main([[maybe_unused]] int argc,
                          [[maybe_unused]] char **argv) {
  auto engine = neko::initializeEngine(configFilePath);
  engine->start();
  delete engine;
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

#endif /* NEKO_INTERFACE_USER_ENTRY_HPP */