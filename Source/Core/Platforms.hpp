#ifndef NEKO_CORE_PLATFORM_HPP
#define NEKO_CORE_PLATFORM_HPP

#include "CoreDefines.hpp"

namespace Neko {

enum Platform { linuxk, windows };

#ifdef __linux__
inline constexpr Platform platform = linuxk;
#elif defined(_WIN64)
inline constexpr Platform platform = windows;
#else
#error "Operating system not supported"
#endif /* current OS */

} /* namespace Neko */

#endif /* NEKO_CORE_PLATFORM_HPP */