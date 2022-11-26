#ifndef VKH_CREATE_OBJECT_HPP
#define VKH_CREATE_OBJECT_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include "utils/utility.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace vkh {
/**
 * @brief Create a Window object
 *
 * @param width
 * @param height
 * @param title
 * @param resizable
 * @param monitor
 * @param share
 * @return GLFWwindow*
 */
GLFWwindow *createWindow(int width, int height,
                         const char *title = "Vulkan Application",
                         bool resizable = true, GLFWmonitor *monitor = nullptr,
                         GLFWwindow *share = nullptr);

/**
 * @brief Create a Instance object
 *
 * @param pCreateInfo
 * @param pAllocator
 * @return VkInstance
 */
VkInstance createInstance(const VkInstanceCreateInfo *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Device object
 *
 * @param physicalDevice
 * @param pCreateInfo
 * @param pAllocator
 * @return VkDevice
 */
VkDevice createDevice(VkPhysicalDevice physicalDevice,
                      const VkDeviceCreateInfo *pCreateInfo,
                      const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Surface object
 *
 * @param instance
 * @param window
 * @param allocator
 * @return VkSurfaceKHR
 */
VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window,
                           const VkAllocationCallbacks *allocator = nullptr);

/**
 * @brief Create a Swapchain object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkSwapchainKHR
 */
VkSwapchainKHR
createSwapchain(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Image View object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkImageView
 */
VkImageView createImageView(VkDevice device,
                            const VkImageViewCreateInfo *pCreateInfo,
                            const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Shader Module object
 *
 * @param device
 * @param shaderCode
 * @param pAllocator
 * @return VkShaderModule
 */
VkShaderModule
createShaderModule(VkDevice device, const std::vector<char> &shaderCode,
                   const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Shader Module object
 *
 * @param device
 * @param shaderCodePath
 * @param pAllocator
 * @return VkShaderModule
 */
VkShaderModule
createShaderModule(VkDevice device, std::string shaderCodePath,
                   const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Pipeline Layout object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkPipelineLayout
 */
VkPipelineLayout
createPipelineLayout(VkDevice device,
                     const VkPipelineLayoutCreateInfo *pCreateInfo,
                     const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Render Pass object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkRenderPass
 */
VkRenderPass
createRenderPass(VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
                 const VkAllocationCallbacks *pAllocator = nullptr);

template <PipelineType pipelineType>
using pipelineInfoType = static_switch<
    pipelineType, VkComputePipelineCreateInfo, VkGraphicsPipelineCreateInfo,
    VkRayTracingPipelineCreateInfoKHR, VkRayTracingPipelineCreateInfoNV>;

/**
 * @brief Create a Pipeline object
 *
 * @tparam pipelineType
 * @param device
 * @param pipelineCache
 * @param createInfos
 * @param pAllocator
 * @return VkPipeline
 */
template <PipelineType pipelineType = Graphics>
VkPipeline createPipeline(VkDevice device, VkPipelineCache pipelineCache,
                          const pipelineInfoType<pipelineType> *createInfo,
                          const VkAllocationCallbacks *pAllocator = nullptr) {
  VkPipeline pipeline;
  VkResult result;
  if constexpr (pipelineType == Compute) {
    result = vkCreateComputePipelines(device, pipelineCache, 1, createInfo,
                                      pAllocator, &pipeline);
  } else if (pipelineType == Graphics) {
    result = vkCreateGraphicsPipelines(device, pipelineCache, 1, createInfo,
                                       pAllocator, &pipeline);
  } else {
    throw std::runtime_error("Unknown pipeline type.");
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipelines.");
  }
  return pipeline;
}

/**
 * @brief Create a Pipelines object
 *
 * @tparam pipelineType
 * @param device
 * @param pipelineCache
 * @param createInfos
 * @param pAllocator
 * @return std::vector<VkPipeline>
 */
template <PipelineType pipelineType = Graphics>
std::vector<VkPipeline>
createPipelines(VkDevice device, VkPipelineCache pipelineCache,
                const std::vector<pipelineInfoType<pipelineType>> &createInfos,
                const VkAllocationCallbacks *pAllocator = nullptr) {
  uint32_t pipelineCount = static_cast<uint32_t>(createInfos.size());
  std::vector<VkPipeline> pipelines{pipelineCount};
  VkResult result;
  if constexpr (pipelineType == Compute) {
    result = vkCreateComputePipelines(device, pipelineCache, pipelineCount,
                                      createInfos.data(), pAllocator,
                                      pipelines.data());
  } else if (pipelineType == Graphics) {
    result = vkCreateGraphicsPipelines(device, pipelineCache, pipelineCount,
                                       createInfos.data(), pAllocator,
                                       pipelines.data());
  } else {
    throw std::runtime_error("Unknown pipeline type.");
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipelines.");
  }
  return pipelines;
}

/**
 * @brief Create a Frame Buffer object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkFramebuffer
 */
VkFramebuffer
createFramebuffer(VkDevice device, const VkFramebufferCreateInfo *pCreateInfo,
                  const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Command Pool object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkCommandPool
 */
VkCommandPool
createCommandPool(VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
                  const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Semaphore object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkSemaphore
 */
VkSemaphore createSemaphore(VkDevice device,
                            const VkSemaphoreCreateInfo *pCreateInfo,
                            const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief Create a Fence object
 *
 * @param device
 * @param pCreateInfo
 * @param pAllocator
 * @return VkFence
 */
VkFence createFence(VkDevice device, const VkFenceCreateInfo *pCreateInfo,
                    const VkAllocationCallbacks *pAllocator = nullptr);

} /* namespace vkh */

#endif /* VK_CREATE_OBJECT_HPP */