#ifndef NEKO_UTILS_SETTINGS_HPP
#define NEKO_UTILS_SETTINGS_HPP

#include "defines.hpp"

namespace neko {

enum CPUThreadUsage {
  low = 3,
  medium = 2,
  high = 1,
};

struct Version {
  u32 major;
  u32 minor;
  u32 patch;
};

struct Settings {
  struct {
    std::string appName = "Vulkan Application";
    u32 appVersion = VK_MAKE_VERSION(1, 0, 0);
    std::string engineName = "Neko Engine";
    u32 engineVersion = VK_MAKE_VERSION(1, 0, 0);
    u32 apiVersion = VK_API_VERSION_1_3;
  } general;

  struct {
    u32 screenWidth = 800;
    u32 screenHeight = 600;

  } graphics;

  struct {
    CPUThreadUsage cpuThreadUsage = high;
  } system;

  Settings() = default;
  Settings(const std::string &settingsFilePath);
};

} /* namespace neko */

#endif /* NEKO_UTILS_SETTINGS_HPP */