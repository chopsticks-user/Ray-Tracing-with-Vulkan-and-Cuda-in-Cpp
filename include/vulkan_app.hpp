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

class VulkanApp {
public:
  VulkanApp() = default;
  ~VulkanApp() = default;
  VulkanApp(const VulkanApp &) = delete;
  VulkanApp(VulkanApp &&) = delete;
  VulkanApp &operator=(const VulkanApp &) = delete;
  VulkanApp &operator=(VulkanApp &&) = delete;

private:
};

} /* namespace rtvc */
#endif /* RTVC_VULKAN_APP_HPP */