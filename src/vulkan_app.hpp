#ifndef RTVC_VULKAN_APP_HPP
#define RTVC_VULKAN_APP_HPP

#include "config.hpp"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif /* GLM_FORCE_RADIANS */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>
#include <unordered_map>

namespace rtvc {

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

  const pGLFWwindow &ref() { return _window; }

private:
  pGLFWwindow _window;
};

class VulkanApp {
public:
  VulkanApp() = default;
  ~VulkanApp() = default;
  VulkanApp(const VulkanApp &) = delete;
  VulkanApp(VulkanApp &&) = delete;
  VulkanApp &operator=(const VulkanApp &) = delete;
  VulkanApp &operator=(VulkanApp &&) = delete;

  void render() {
    while (!glfwWindowShouldClose(_window.ref())) {
      glfwPollEvents();
    }
  }

private:
  vk::raii::Context _context = {};
  rtvc::GLFW _glfw = {};

  rtvc::Window _window = {};

  [[nodiscard]] vk::raii::Instance makeInstance();
  vk::raii::Instance _instance = makeInstance();

  [[nodiscard]] vk::raii::DebugUtilsMessengerEXT makeDebugMessenger();
  // vk::raii::DebugUtilsMessengerEXT debugMessenger = makeDebugMessenger();
};

} /* namespace rtvc */
#endif /* RTVC_VULKAN_APP_HPP */