#ifndef VKH_WRAPPER_HPP
#define VKH_WRAPPER_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

namespace vkh {
struct ShaderModuleWrapper {
  static constexpr uint32_t stageCount = 2;

  VkShaderModule vertex;
  VkShaderModule fragment;
};

struct GraphicsPipelineWrapper {
  VkPipeline self = nullptr;
  VkPipelineLayout layout = nullptr;
  VkPipelineCache cache = nullptr;
  VkRenderPass renderPass = nullptr;
};

struct SwapChainWrapper {
  VkSwapchainKHR self;
  VkFormat format;
  VkExtent2D extent;
};

} /* namespace vkh */

#endif /* VKH_WRAPPER_HPP */