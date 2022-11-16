#ifndef VKH_VALIDATION_LAYERS_HPP
#define VKH_VALIDATION_LAYERS_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace vkh {

bool checkValidationLayerSupport();

} /* namespace vkh */

#endif /* VKH_VALIDATION_LAYERS_HPP */