#ifndef VKH_UTILITY_HPP
#define VKH_UTILITY_HPP

#include <tuple>

namespace vkh {
enum PipelineType { Compute, Graphics, RayTracingKHR, RayTracingNV };
enum PositionDim { P2, P3, P4 };
enum ColorDim { C3, C4 };

template <size_t index, typename... Args>
using static_switch =
    typename std::tuple_element<index, std::tuple<Args...>>::type;

} // namespace vkh

#endif /* VKH_UTILITY_HPP */