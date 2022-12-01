#ifndef VKH_VERTEX_HPP
#define VKH_VERTEX_HPP

#include "config.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */
#include <glm/glm.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#endif /* GLM_ENABLE_EXPERIMENTAL */

#include <array>

namespace vkw {

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texCoord;

  static constexpr size_t attributeCount = 3;

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
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }

  bool operator==(const Vertex &rhs) const {
    return position == rhs.position && color == rhs.color &&
           texCoord == rhs.texCoord;
  }
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

} // namespace vkw

namespace std {
template <> struct hash<vkw::Vertex> {
  size_t operator()(vkw::Vertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.position) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) &
           (hash<glm::vec2>()(vertex.texCoord) << 1);
  }
};
} // namespace std

#endif /* VKH_VERTEX_HPP */