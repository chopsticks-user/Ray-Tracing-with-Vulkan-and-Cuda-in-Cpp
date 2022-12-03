#ifndef RTVC_CONFIG_HPP
#define RTVC_CONFIG_HPP

#ifdef NDEBUG
#ifndef VULKAN_HPP_NO_SPACESHIP_OPERATOR
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR
#endif /* VULKAN_HPP_NO_SPACESHIP_OPERATOR */
#ifndef VULKAN_HPP_NO_TO_STRING
#define VULKAN_HPP_NO_TO_STRING
#endif /* VULKAN_HPP_NO_TO_STRING */
#endif /* NDEBUG */

#ifndef VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_CONSTRUCTORS
#endif /* VULKAN_HPP_NO_CONSTRUCTORS */

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace rtvc::config {

#ifdef NDEBUG
inline constexpr bool debugMode = false;
#else
inline constexpr bool debugMode = true;
#endif /* NDEBUG */

} /* namespace rtvc::config */

#endif /* RTVC_CONFIG_HPP */