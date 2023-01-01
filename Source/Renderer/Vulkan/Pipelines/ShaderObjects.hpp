#ifndef HDG_RENDERER_VULKAN_PIPELINES_SHADER_OBJECTS_HPP
#define HDG_RENDERER_VULKAN_PIPELINES_SHADER_OBJECTS_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif /* GLM_FORCE_RADIANS */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#endif /* GLM_ENABLE_EXPERIMENTAL */

#include <array>

namespace Neko::Internal::VK
{
    namespace ShaderObject
    {

        typedef u32 Index;

        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec2 texCoord;

            static constexpr size_t attributeCount = 3;

            static VkVertexInputBindingDescription getBindingDescription();

            static std::array<VkVertexInputAttributeDescription, attributeCount>
            getAttributeDescriptions();

            bool operator==(const Vertex &rhs) const
            {
                return position == rhs.position && color == rhs.color &&
                       texCoord == rhs.texCoord;
            }
        };

        struct Uniform
        {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

    } /* namespace ShaderObject */
} // namespace Neko::Internal::VK

namespace std
{
    template <>
    struct hash<Neko::Internal::VK::ShaderObject::Vertex>
    {
        size_t operator()(Neko::Internal::VK::ShaderObject::Vertex const &vertex) const
        {
            return ((hash<glm::vec3>()(vertex.position) ^
                     (hash<glm::vec3>()(vertex.color) << 1)) >>
                    1) &
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
} /* namespace std */

#endif /* HDG_RENDERER_VULKAN_PIPELINES_SHADER_OBJECTS_HPP */