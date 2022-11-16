#ifndef RTVC_VERTEX_HPP
#define RTVC_VERTEX_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>

namespace rtvc {
struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription();

  static std::array<VkVertexInputAttributeDescription, 2>
  getAttributeDescriptions();
};
} /* namespace rtvc */

#endif /* RTVC_VERTEX_HPP */