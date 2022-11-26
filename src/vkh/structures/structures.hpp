#ifndef VKH_VERTEX_HPP
#define VKH_VERTEX_HPP

#include "config.hpp"
#include "utils/utility.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <array>
#include <glm/glm.hpp>

namespace vkh {

template <PositionDim positionDim = P3, ColorDim colorDim = C3> struct Vertex {
  static_assert(positionDim >= P2 && positionDim <= P4);
  static_assert(colorDim >= C3 && colorDim <= C4);

  using PosVecType =
      static_switch<positionDim, glm::vec2, glm::vec3, glm::vec4>;
  using ColorVecType = static_switch<colorDim, glm::vec3, glm::vec4>;

  PosVecType position;
  ColorVecType color;

  static constexpr size_t attributeCount = 2;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, attributeCount>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, attributeCount>
        attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

typedef Vertex<P2, C3> Vertex2D_RGB;
typedef Vertex<P2, C4> Vertex2D_RGBA;
typedef Vertex<P3, C3> Vertex3D_RGB;
typedef Vertex<P3, C4> Vertex3D_RGBA;

} /* namespace vkh */

#endif /* VKH_VERTEX_HPP */