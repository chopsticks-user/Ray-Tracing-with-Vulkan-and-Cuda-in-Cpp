#ifndef NEKO_SETTINGS_HPP
#define NEKO_SETTINGS_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace neko {

#ifndef NDEBUG
inline constexpr bool debugMode = true;
#else
inline constexpr bool debugMode = false;
#endif /* NDEBUG */

struct StaticObject {
  StaticObject() = default;
  StaticObject(const StaticObject &) = delete;
  StaticObject(StaticObject &&) = delete;
  StaticObject &operator=(const StaticObject &) = delete;
  StaticObject &operator=(StaticObject &&) = delete;
  virtual ~StaticObject() = default;
};

struct MoveOnlyObject {
  MoveOnlyObject() = default;
  MoveOnlyObject(const MoveOnlyObject &) = delete;
  MoveOnlyObject(MoveOnlyObject &&) = default;
  MoveOnlyObject &operator=(const MoveOnlyObject &) = delete;
  MoveOnlyObject &operator=(MoveOnlyObject &&) = default;
  virtual ~MoveOnlyObject() = default;
};

struct Settings {
  struct {
    std::string appName = "Vulkan Application";
    uint32_t appVersion = VK_MAKE_VERSION(1, 0, 0);
    std::string engineName = "Neko Engine";
    uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
    uint32_t apiVersion = VK_API_VERSION_1_3;
  } general;
  struct {
    uint32_t screenWidth = 800;
    uint32_t screenHeight = 600;

    bool enableRaytracing = false;
  } graphics;
};

} /* namespace neko */

#endif /* NEKO_SETTINGS_HPP */