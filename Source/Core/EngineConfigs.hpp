#ifndef NEKO_CORE_CONFIGS_HPP
#define NEKO_CORE_CONFIGS_HPP

#include "CoreDefines.hpp"

namespace neko {
struct Version {
  u32 major;
  u32 minor;
  u32 patch;
};

enum class CPUThreadUsage {
  low = 3,
  medium = 2,
  high = 1,
};

enum class PresentMode {
  immediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
  mailbox = VK_PRESENT_MODE_MAILBOX_KHR,
  fifo = VK_PRESENT_MODE_FIFO_KHR,
  fifoRelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
};

struct EngineConfigs {
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
    PresentMode presentMode = PresentMode::fifo;

  } graphics;

  struct {
    CPUThreadUsage cpuThreadUsage = CPUThreadUsage::high;
  } system;

  EngineConfigs() = default;
  EngineConfigs(const std::string &configsFilePath);
};

} /* namespace neko */

#endif /* NEKO_CORE_CONFIGS_HPP */