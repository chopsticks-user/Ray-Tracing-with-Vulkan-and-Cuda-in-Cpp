#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <vector>
#include <vkh.hpp>

namespace shader {

const std::vector<vkh::Vertex2D_RGB> triangle{
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

} /* namespace shader */

namespace texture {} /* namespace texture */

#endif /* RESOURCES_HPP */