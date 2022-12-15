#include "settings.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <map>

namespace neko {

static Version getVersion(const std::string &versionStr) {
  u32 versionParts[3] = {0, 0, 0};
  u64 versionStrLength = versionStr.length();
  u64 currentIndex;
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

static u32 makeVkVersion(const std::string &versionStr) {
  Version version = getVersion(versionStr);
  return VK_MAKE_VERSION(version.major, version.minor, version.patch);
}

Settings::Settings(const std::string &settingsFilePath) {
  std::fstream fs(settingsFilePath);
  if (!fs.is_open()) {
    throw std::runtime_error("Failed to open settings file " +
                             settingsFilePath);
  }
  auto jsonData = nlohmann::json::parse(fs);

  auto generalSettings = jsonData["general"];
  general.appName = generalSettings["info"]["application"]["name"];
  general.appVersion =
      makeVkVersion(generalSettings["info"]["application"]["version"]);
  general.engineName = generalSettings["info"]["engine"]["name"];
  general.engineVersion =
      makeVkVersion(generalSettings["info"]["engine"]["version"]);
  general.apiVersion = makeVkVersion(generalSettings["info"]["api"]["version"]);

  auto graphicsSettings = jsonData["graphics"];
  graphics.screenWidth = graphicsSettings["render-window"]["width"];
  graphics.screenHeight = graphicsSettings["render-window"]["height"];

  auto systemSettings = jsonData["system"];
  std::map<std::string, CPUThreadUsage> cpuThreadUsageOptions{
      {"high", high},
      {"medium", medium},
      {"low", low},
  };
  system.cpuThreadUsage =
      cpuThreadUsageOptions.at(systemSettings["cpu-thread-usage"]);

  auto advancedSettings = jsonData["advanced"];
}

} /* namespace neko */