#ifndef HDG_CORE_CONFIGS_ENGINECONFIGS_HPP
#define HDG_CORE_CONFIGS_ENGINECONFIGS_HPP

#include "Common/Common.hpp"

namespace Neko::Core
{
    struct Version
    {
        u32 major;
        u32 minor;
        u32 patch;
    };

    namespace Enum
    {
        enum class CPUThreadUsage
        {
            low = 3,
            medium = 2,
            high = 1,
        };

        enum class PresentMode
        {
            immediate = 0,
            mailbox = 1,
            fifo = 2,
            fifoRelaxed = 3,
        };

    } // namespace Neko::Core::Enum

    struct EngineConfigs
    {
        struct
        {
            std::string appName = "Vulkan Application";
            Version appVersion = {1, 0, 0};
            std::string engineName = "Neko Engine";
            Version engineVersion = {1, 0, 0};
            Version apiVersion = {1, 3, 0};
        } general;

        struct
        {
            u32 screenWidth = 800;
            u32 screenHeight = 600;
            Enum::PresentMode presentMode = Enum::PresentMode::immediate;

        } graphics;

        struct
        {
            Enum::CPUThreadUsage cpuThreadUsage = Enum::CPUThreadUsage::high;
        } system;

        EngineConfigs() = default;

        EngineConfigs(const std::string &configsFilePath);
    };

} /* namespace Neko::Core */

#endif /* HDG_CORE_CONFIGS_ENGINECONFIGS_HPP */