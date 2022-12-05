#ifndef RTVC_BASIC_INSTANCE_HPP
#define RTVC_BASIC_INSTANCE_HPP

#include "config.hpp"

namespace rtvc {

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              [[maybe_unused]] void *pUserData);

[[nodiscard]] vk::raii::Instance makeInstance(const vk::raii::Context &context);

[[nodiscard]] vk::DebugUtilsMessengerCreateInfoEXT makeDebugMessengerInfo();

[[nodiscard]] vk::raii::DebugUtilsMessengerEXT
makeDebugMessenger(const vk::raii::Instance &instance);

bool validationLayerSupported();

vk::raii::SurfaceKHR makeSurface(const vk::raii::Instance &instance,
                                 const Window &window);

} /* namespace rtvc */
#endif /* RTVC_BASIC_INSTANCE_HPP */