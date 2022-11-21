#include "destroy_object.hpp"

namespace vkh {

void destroyWindow(GLFWwindow *window) { glfwDestroyWindow(window); }

void destroyInstance(VkInstance instance,
                     const VkAllocationCallbacks *pAllocator) {
  vkDestroyInstance(instance, pAllocator);
}

void destroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
  vkDestroyDevice(device, pAllocator);
}

void destroySurface(VkInstance instance, VkSurfaceKHR surface,
                    const VkAllocationCallbacks *pAllocator) {
  vkDestroySurfaceKHR(instance, surface, pAllocator);
}

void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain,
                      const VkAllocationCallbacks *pAllocator) {
  vkDestroySwapchainKHR(device, swapchain, pAllocator);
}

void destroyImageView(VkDevice device, VkImageView imageView,
                      const VkAllocationCallbacks *pAllocator) {
  vkDestroyImageView(device, imageView, pAllocator);
}

void destroyShaderModule(VkDevice device, VkShaderModule shaderModule,
                         const VkAllocationCallbacks *pAllocator) {
  vkDestroyShaderModule(device, shaderModule, pAllocator);
}

void destroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout,
                           const VkAllocationCallbacks *pAllocator) {
  vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
}

void destroyRenderPass(VkDevice device, VkRenderPass renderPass,
                       const VkAllocationCallbacks *pAllocator) {
  vkDestroyRenderPass(device, renderPass, pAllocator);
}

void destroyPipeline(VkDevice device, VkPipeline pipeline,
                     const VkAllocationCallbacks *pAllocator) {
  vkDestroyPipeline(device, pipeline, pAllocator);
}

void destroyPipelines(VkDevice device, std::vector<VkPipeline> pipelines,
                      const VkAllocationCallbacks *pAllocator) {
  for (auto &pipeline : pipelines) {
    vkDestroyPipeline(device, pipeline, pAllocator);
  }
}

void destroyCommandPool(VkDevice device, VkCommandPool commandPool,
                        const VkAllocationCallbacks *pAllocator) {
  vkDestroyCommandPool(device, commandPool, pAllocator);
}

} /* namespace vkh */