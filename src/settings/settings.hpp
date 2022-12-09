#ifndef NEKO_SETTINGS_HPP
#define NEKO_SETTINGS_HPP

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

typedef _Float32 f32;
typedef _Float64 f64;
typedef _Float128 f128;

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
static_assert(sizeof(f128) == 16, "sizeof(f128) != 16 bytes");

static_assert(sizeof(b8) == 1, "sizeof(u16) != 1 byte");
static_assert(sizeof(b32) == 4, "sizeof(b32) != 4 bytes");

#define vku32(x) static_cast<u32>(x)

namespace neko {

#ifndef NDEBUG
inline constexpr bool debugMode = true;
#else
inline constexpr bool debugMode = false;
#endif /* NDEBUG */

struct Settings {
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

  } graphics;
};

} /* namespace neko */

#endif /* NEKO_SETTINGS_HPP */