#ifndef VKH_DESTROY_OBJECT_HPP
#define VKH_DESTROY_OBJECT_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <vector>

namespace vkh {
/**
 * @brief
 *
 * @param window
 */
void destroyWindow(GLFWwindow *window);

/**
 * @brief
 *
 * @param instance
 * @param pAllocator
 */
void destroyInstance(VkInstance instance,
                     const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param pAllocator
 */
void destroyDevice(VkDevice device,
                   const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param instance
 * @param surface
 * @param pAllocator
 */
void destroySurface(VkInstance instance, VkSurfaceKHR surface,
                    const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param swapchain
 * @param pAllocator
 */
void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain,
                      const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param imageView
 * @param pAllocator
 */
void destroyImageView(VkDevice device, VkImageView imageView,
                      const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param shaderModule
 * @param pAllocator
 */
void destroyShaderModule(VkDevice device, VkShaderModule shaderModule,
                         const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param pipelineLayout
 * @param pCallbacks
 */
void destroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout,
                           const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param renderPass
 * @param pAllocator
 */
void destroyRenderPass(VkDevice device, VkRenderPass renderPass,
                       const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param pipeline
 * @param pAllocator
 * @return * void
 */
void destroyPipeline(VkDevice device, VkPipeline pipeline,
                     const VkAllocationCallbacks *pAllocator = nullptr);

void destroyPipelines(VkDevice device, std::vector<VkPipeline> pipelines,
                      const VkAllocationCallbacks *pAllocator = nullptr);

/**
 * @brief
 *
 * @param device
 * @param commandPool
 * @param pAllocator
 */
void destroyCommandPool(VkDevice device, VkCommandPool commandPool,
                        const VkAllocationCallbacks *pAllocator = nullptr);

} /* namespace vkh */

#endif /* VKH_DESTROY_OBJECT_HPP */