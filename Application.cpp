#include <Neko.hpp>

#include <filesystem>
#include <iosfwd>
#include <iostream>

static const std::string settingsFilePath =
    std::filesystem::current_path().string() + "/Data/Configs/settings.json";

static const std::string vertexShaderPath =
    std::filesystem::current_path().string() +
    "/Data/Resources/Shaders/Basic.vert.spv";
static const std::string fragmentShaderPath =
    std::filesystem::current_path().string() +
    "/Data/Resources/Shaders/Basic.frag.spv";

static int protected_main([[maybe_unused]] int argc,
                          [[maybe_unused]] char **argv) {
  TIMER_START(t);
  auto engine = std::make_unique<neko::Engine>(settingsFilePath);
  TIMER_INVOKE(t, "Engine load time");
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