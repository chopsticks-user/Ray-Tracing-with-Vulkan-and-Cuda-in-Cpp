#ifndef UTILS_PLATFORM_HPP
#define UTILS_PLATFORM_HPP

#include "defines.hpp"

namespace neko {

enum Platform { linuxk, windows };

#ifdef __linux__
inline constexpr Platform platform = linuxk;
#elif defined(_WIN64)
inline constexpr Platform platform = windows;
#else
#error "Operating system not supported"
#endif /* current OS */

} /* namespace neko */

#endif /* UTILS_PLATFORM_HPP */