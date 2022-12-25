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
            immediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
            mailbox = VK_PRESENT_MODE_MAILBOX_KHR,
            fifo = VK_PRESENT_MODE_FIFO_KHR,
            fifoRelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
        };

    } // namespace Neko::Core::Enum

    struct EngineConfigs
    {
        struct
        {
            std::string appName = "Vulkan Application";
            u32 appVersion = VK_MAKE_VERSION(1, 0, 0);
            std::string engineName = "Neko Engine";
            u32 engineVersion = VK_MAKE_VERSION(1, 0, 0);
            u32 apiVersion = VK_API_VERSION_1_3;
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