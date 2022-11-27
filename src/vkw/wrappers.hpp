#ifndef VKW_WRAPPERS_HPP
#define VKW_WRAPPERS_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

namespace vkw {

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

#endif /* VKW_WRAPPERS_HPP */