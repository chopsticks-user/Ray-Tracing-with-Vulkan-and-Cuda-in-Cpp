#ifndef RTVC_BASIC_DEVICE_HPP
#define RTVC_BASIC_DEVICE_HPP

#include "config.hpp"

namespace rtvc {

DeviceWrapper makeLogicalDevice(const vk::raii::Instance &instance,
                                const vk::raii::SurfaceKHR &surface);

} /* namespace rtvc */
#endif /* RTVC_BASIC_DEVICE_HPP */