#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <vector>
#include <vkh.hpp>

namespace shader {

const std::vector<uint16_t> triangle_indices{0, 1, 2, 2, 3, 0};
const std::vector<vkh::Vertex2D_RGB> triangle_index_data{
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

} /* namespace shader */

namespace texture {} /* namespace texture */

#endif /* RESOURCES_HPP */