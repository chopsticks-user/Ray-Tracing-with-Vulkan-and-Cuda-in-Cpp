#ifndef RTVC_RESOURCES_SHADERS_TRIANGLE
#define RTVC_RESOURCES_SHADERS_TRIANGLE

#include "Vertex.hpp"
#include <vector>

namespace rtvc::resources::shaders {
inline const std::vector<rtvc::Vertex> triangle = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

inline const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
} /* namespace rtvc::resources::shaders */

#endif /* RTVC_RESOURCES_SHADERS_TRIANGLE */