#ifndef HDG_CORE_PLATFORMS_HPP
#define HDG_CORE_PLATFORMS_HPP

#include "Common/Common.hpp"

namespace Neko::Core
{
    namespace Enum
    {
        enum class Platform
        {
            linuxk,
            windows
        };

    } // namespace Neko::Core::Enum

#ifdef __linux__
    inline constexpr Enum::Platform platform = Enum::Platform::linuxk;
#elif defined(_WIN64)
    inline constexpr Enum::Platform platform = Enum::Platform::windows;
#else
#error "Operating system currently not supported"
#endif /* current OS */

} /* namespace Neko::Core */

#endif /* HDG_CORE_PLATFORMS_HPP */