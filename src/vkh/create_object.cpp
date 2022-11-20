#include "create_object.hpp"

namespace vkh {
GLFWwindow *createWindow(int width, int height, const char *title,
                         GLFWmonitor *monitor, GLFWwindow *share) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  return glfwCreateWindow(width, height, title, monitor, share);
}

VkInstance createInstance(const VkInstanceCreateInfo *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator) {
  VkInstance instance;
  if (vkCreateInstance(pCreateInfo, pAllocator, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create an instance.");
  }
  return instance;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice,
                      const VkDeviceCreateInfo *pCreateInfo,
                      const VkAllocationCallbacks *pAllocator) {
  VkDevice device;
  if (vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, &device) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device.");
  }
  return device;
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window,
                           const VkAllocationCallbacks *allocator) {
  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(instance, window, allocator, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface.");
  }
  return surface;
}

VkSwapchainKHR createSwapchain(VkDevice device,
                               const VkSwapchainCreateInfoKHR *pCreateInfo,
                               const VkAllocationCallbacks *pAllocator) {
  VkSwapchainKHR swapchain{};
  if (vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, &swapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create a swapchain.");
  }
  return swapchain;
}

VkImageView createImageView(VkDevice device,
                            const VkImageViewCreateInfo *pCreateInfo,
                            const VkAllocationCallbacks *pAllocator) {
  VkImageView imageView;
  if (vkCreateImageView(device, pCreateInfo, pAllocator, &imageView) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create an image view.");
  }
  return imageView;
}

VkPipeline createComputePipeline(VkDevice device, VkPipelineCache pipelineCache,
                                 const VkComputePipelineCreateInfo *pCreateInfo,
                                 const VkAllocationCallbacks *pAllocator) {
  VkPipeline computePipeline;
  if (vkCreateComputePipelines(device, pipelineCache, 1, pCreateInfo,
                               pAllocator, &computePipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create a compute pipeline.");
  }
  return computePipeline;
}

std::vector<VkPipeline>
createComputePipelines(VkDevice device, VkPipelineCache pipelineCache,
                       std::vector<VkComputePipelineCreateInfo> createInfos,
                       const VkAllocationCallbacks *pAllocator) {
  size_t createInfoCount = createInfos.size();
  std::vector<VkPipeline> computePipelines{createInfoCount};
  if (vkCreateComputePipelines(device, pipelineCache,
                               static_cast<uint32_t>(createInfoCount),
                               createInfos.data(), pAllocator,
                               computePipelines.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create compute pipelines.");
  }
  return computePipelines;
}

VkPipeline
createGraphicsPipeline(VkDevice device, VkPipelineCache pipelineCache,
                       const VkGraphicsPipelineCreateInfo *pCreateInfo,
                       const VkAllocationCallbacks *pAllocator) {
  VkPipeline graphicsPipeline;
  if (vkCreateGraphicsPipelines(device, pipelineCache, 1, pCreateInfo,
                                pAllocator, &graphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create a graphics pipeline.");
  }
  return graphicsPipeline;
}

std::vector<VkPipeline>
createGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache,
                        std::vector<VkGraphicsPipelineCreateInfo> createInfos,
                        const VkAllocationCallbacks *pAllocator) {
  size_t createInfoCount = createInfos.size();
  std::vector<VkPipeline> graphicsPipelines{createInfoCount};
  if (vkCreateGraphicsPipelines(device, pipelineCache,
                                static_cast<uint32_t>(createInfoCount),
                                createInfos.data(), pAllocator,
                                graphicsPipelines.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipelines.");
  }
  return graphicsPipelines;
}

VkCommandPool createCommandPool(VkDevice device,
                                const VkCommandPoolCreateInfo *pCreateInfo,
                                const VkAllocationCallbacks *pAllocator) {
  VkCommandPool commandPool{};
  if (vkCreateCommandPool(device, pCreateInfo, pAllocator, &commandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
  return commandPool;
}

} /* namespace vkh */