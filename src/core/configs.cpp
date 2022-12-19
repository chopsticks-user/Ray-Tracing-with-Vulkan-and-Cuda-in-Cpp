#include "configs.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <map>

namespace neko {

static Version getVersion(const std::string &versionStr) {
  u32 versionParts[3] = {0, 0, 0};
  u64 versionStrLength = versionStr.length();
  u64 currentIndex = 0;
  for (u64 i = 0; i < 3; ++i) {
    std::string numStr = "0";
    for (; currentIndex < versionStrLength; ++currentIndex) {
      if (versionStr[currentIndex] == '.') {
        currentIndex = currentIndex;
        break;
      }
      numStr += versionStr[currentIndex];
    }
    versionParts[i] = std::stoi(numStr);
  }
  return {
      versionParts[0],
      versionParts[1],
      versionParts[2],
  };
}

static u32 getVkVersion(const std::string &versionStr) {
  Version version = getVersion(versionStr);
  return VK_MAKE_VERSION(version.major, version.minor, version.patch);
}

static CPUThreadUsage getCPUThreadUsage(const std::string &usageModeStr) {
  if (usageModeStr == "high") {
    return CPUThreadUsage::high;
  }
  if (usageModeStr == "medium") {
    return CPUThreadUsage::medium;
  }
  if (usageModeStr == "low") {
    return CPUThreadUsage::low;
  }
  throw std::runtime_error("Unknown CPU thread usage mode.");
}

static PresentMode getPresentMode(const std::string &presentModeStr) {
  if (presentModeStr == "immediate") {
    return PresentMode::immediate;
  }
  if (presentModeStr == "mailbox") {
    return PresentMode::mailbox;
  }
  if (presentModeStr == "fifo") {
    return PresentMode::fifo;
  }
  if (presentModeStr == "fifo-relaxed") {
    return PresentMode::fifoRelaxed;
  }
  throw std::runtime_error("Unknown presentation mode.");
}

Configs::Configs(const std::string &configsFilePath) {
  std::fstream fs(configsFilePath);
  if (!fs.is_open()) {
    *this = Configs{};
    std::cout << "Failed to find a settings file, using default settings\n";
    return;
  }
  auto jsonData = nlohmann::json::parse(fs);

  auto generalSettings = jsonData["general"];
  general.appName = generalSettings["info"]["application"]["name"];
  general.appVersion =
      getVkVersion(generalSettings["info"]["application"]["version"]);
  general.engineName = generalSettings["info"]["engine"]["name"];
  general.engineVersion =
      getVkVersion(generalSettings["info"]["engine"]["version"]);
  general.apiVersion = getVkVersion(generalSettings["info"]["api"]["version"]);

  auto graphicsSettings = jsonData["graphics"];
  graphics.screenWidth = graphicsSettings["render-window"]["width"];
  graphics.screenHeight = graphicsSettings["render-window"]["height"];
  graphics.presentMode = getPresentMode(graphicsSettings["present-mode"]);

  auto systemSettings = jsonData["system"];
  system.cpuThreadUsage = getCPUThreadUsage(systemSettings["cpu-thread-usage"]);

  auto advancedSettings = jsonData["advanced"];
}

} /* namespace neko */