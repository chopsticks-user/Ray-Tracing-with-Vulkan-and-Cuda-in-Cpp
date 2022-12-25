#include "EngineConfigs.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <map>

namespace Neko::Core
{
    static Version getVersion(const std::string &versionStr)
    {
        u32 versionParts[3] = {0, 0, 0};
        u64 versionStrLength = versionStr.length();
        u64 currentIndex = 0;
        for (u64 i = 0; i < 3; ++i)
        {
            std::string numStr = "0";
            for (; currentIndex < versionStrLength; ++currentIndex)
            {
                if (versionStr[currentIndex] == '.')
                {
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

    static u32 getVkVersion(const std::string &versionStr)
    {
        Version version = getVersion(versionStr);
        return VK_MAKE_VERSION(version.major, version.minor, version.patch);
    }

    static Enum::CPUThreadUsage getCPUThreadUsage(const std::string &usageModeStr)
    {
        if (usageModeStr == "high")
        {
            return Enum::CPUThreadUsage::high;
        }
        if (usageModeStr == "medium")
        {
            return Enum::CPUThreadUsage::medium;
        }
        if (usageModeStr == "low")
        {
            return Enum::CPUThreadUsage::low;
        }
        throw std::runtime_error("Unknown CPU thread usage mode.");
    }

    static Enum::PresentMode getPresentMode(const std::string &presentModeStr)
    {
        if (presentModeStr == "immediate")
        {
            return Enum::PresentMode::immediate;
        }
        if (presentModeStr == "mailbox")
        {
            return Enum::PresentMode::mailbox;
        }
        if (presentModeStr == "fifo")
        {
            return Enum::PresentMode::fifo;
        }
        if (presentModeStr == "fifo-relaxed")
        {
            return Enum::PresentMode::fifoRelaxed;
        }
        throw std::runtime_error("Unknown presentation mode.");
    }

    EngineConfigs::EngineConfigs(const std::string &configsFilePath)
    {
        std::fstream fs(configsFilePath);
        if (!fs.is_open())
        {
            *this = EngineConfigs{};
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

} /* namespace Neko::Core */