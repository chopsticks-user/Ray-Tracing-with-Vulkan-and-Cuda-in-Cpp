#ifndef RTVC_BASIC_COMMAND_POOL_HPP
#define RTVC_BASIC_COMMAND_POOL_HPP

#include "config.hpp"

namespace rtvc {

vk::raii::CommandPool makeCommandPool(const DeviceWrapper &device);

} /* namespace rtvc */
#endif /* RTVC_BASIC_COMMAND_POOL_HPP */