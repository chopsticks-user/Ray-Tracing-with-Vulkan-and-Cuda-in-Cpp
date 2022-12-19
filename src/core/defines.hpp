#ifndef NEKO_CORE_DEFINES_HPP
#define NEKO_CORE_DEFINES_HPP

#include "configs.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// #include <vulkan/vulkan.hpp>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef char b8;
typedef int32_t b32;

static_assert(sizeof(u8) == 1, "sizeof(u8) != 1 byte");
static_assert(sizeof(u16) == 2, "sizeof(u16) != 2 bytes");
static_assert(sizeof(u32) == 4, "sizeof(u32) != 4 bytes");
static_assert(sizeof(u64) == 8, "sizeof(u8) != 8 bytes");

static_assert(sizeof(i8) == 1, "sizeof(i8) != 1 byte");
static_assert(sizeof(i16) == 2, "sizeof(i16) != 2 bytes");
static_assert(sizeof(i32) == 4, "sizeof(i32) != 4 bytes");
static_assert(sizeof(i64) == 8, "sizeof(i8) != 8 bytes");

static_assert(sizeof(f32) == 4, "sizeof(f32) != 4 bytes");
static_assert(sizeof(f64) == 8, "sizeof(f64) != 8 bytes");

static_assert(sizeof(b8) == 1, "sizeof(u16) != 1 byte");
static_assert(sizeof(b32) == 4, "sizeof(b32) != 4 bytes");

#define vku32(x) static_cast<u32>(x)
#define stdu64(x) static_cast<size_t>(x)

namespace neko {

#ifndef NDEBUG
inline constexpr bool debugMode = true;
#else
inline constexpr bool debugMode = false;
#endif /* NDEBUG */

enum CppVersion { std17 = 17, std20 = 20 };

#if __cplusplus > 201703L
inline constexpr CppVersion cppVersion = std20;
#elif __cplusplus > 201402L
inline constexpr CppVersion cppVersion = std17;
#else
#error "At least C++17 is required"
#endif /* C++ version */

} /* namespace neko */

#endif /* NEKO_CORE_DEFINES_HPP */