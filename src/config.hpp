#ifndef RTVC_CONFIG_HPP
#define RTVC_CONFIG_HPP

#ifdef NDEBUG
#ifndef VULKAN_HPP_NO_SPACESHIP_OPERATOR
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR
#endif /* VULKAN_HPP_NO_SPACESHIP_OPERATOR */
#ifndef VULKAN_HPP_NO_TO_STRING
#define VULKAN_HPP_NO_TO_STRING
#endif /* VULKAN_HPP_NO_TO_STRING */
#endif /* NDEBUG */

#ifndef VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_CONSTRUCTORS
#endif /* VULKAN_HPP_NO_CONSTRUCTORS */

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #ifndef GLM_FORCE_RADIANS
// #define GLM_FORCE_RADIANS
// #endif /* GLM_FORCE_RADIANS */
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

namespace rtvc {

#ifdef NDEBUG
inline constexpr bool debugMode = false;
#else
inline constexpr bool debugMode = true;
#endif /* NDEBUG */

inline constexpr std::string_view validationLayerName =
    "VK_LAYER_KHRONOS_validation";
inline constexpr std::string_view mangohudLayerName =
    "VK_LAYER_MANGOHUD_overlay";

struct GLFW {
  GLFW() { glfwInit(); }
  ~GLFW() { glfwTerminate(); }
  GLFW(const GLFW &) = delete;
  GLFW(GLFW &&) = delete;
  GLFW &operator=(const GLFW &) = delete;
  GLFW &operator=(GLFW &&) = delete;
};

class Window {
  typedef GLFWwindow *pGLFWwindow;

public:
  Window() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(800, 600, "VulkanApplication", nullptr, nullptr);
  }
  ~Window() { glfwDestroyWindow(_window); }
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;

  pGLFWwindow operator*() const noexcept { return _window; }

private:
  pGLFWwindow _window;
};

struct DeviceWrapper {
  uint32_t queueFamilyIndex;
  vk::raii::Queue queue;
  vk::raii::PhysicalDevice physical = VK_NULL_HANDLE;
  vk::raii::Device logical = VK_NULL_HANDLE;
};

struct SwapchainWrapper {
  vk::raii::SwapchainKHR self;
  vk::Format format;
  vk::Extent2D extent;
};

struct DepthBufferWrapper {
  vk::Format format;
  vk::raii::Image image;
  vk::raii::ImageView imageView;
};

struct Settings {
  const uint32_t maxFramesInFlight = 2;

  vk::PresentModeKHR presentMode = vk::PresentModeKHR::eImmediate;

  vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;
  bool msaaOn() const noexcept {
    return msaaSamples != vk::SampleCountFlagBits::e1;
  }
};

} /* namespace rtvc */

#endif /* RTVC_CONFIG_HPP */