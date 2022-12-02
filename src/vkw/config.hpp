#ifndef VKW_CONFIG_HPP
#define VKW_CONFIG_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <stdexcept>

#ifndef CUSTOM
#define CUSTOM
#endif /* CUSTOM */

namespace vkw {

#ifdef NDEBUG
inline constexpr bool enableValidationLayers = false;
#else
inline constexpr bool enableValidationLayers = true;
#endif /* NDEBUG */

struct ShaderModuleWrapper {
  static constexpr uint32_t stageCount = 2;

  VkShaderModule vertex;
  VkShaderModule fragment;
};

struct GraphicsPipelineDepWrapper {
  VkPipelineLayout layout = nullptr;
  VkPipelineCache cache = nullptr;
  VkRenderPass renderPass = nullptr;
};

struct SwapChainWrapper {
  VkSwapchainKHR self;
  VkFormat format;
  VkExtent2D extent;
};

struct MultiPurposeQueue {
  VkQueue ref;
  uint32_t familyIndex;
};

struct DeviceReturnWrapper {
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  MultiPurposeQueue queue;
};

} /* namespace vkw */
#endif /* VKW_CONFIG_HPP */