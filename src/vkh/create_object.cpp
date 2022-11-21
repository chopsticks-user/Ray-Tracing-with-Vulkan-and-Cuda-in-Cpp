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

VkShaderModule createShaderModule(VkDevice device,
                                  const std::vector<char> &shaderCode,
                                  const VkAllocationCallbacks *pAllocator) {
  VkShaderModuleCreateInfo shaderModuleInfo{};
  shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderModuleInfo.codeSize = shaderCode.size();
  shaderModuleInfo.pCode =
      reinterpret_cast<const uint32_t *>(shaderCode.data());
  shaderModuleInfo.pNext = nullptr;

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &shaderModuleInfo, pAllocator,
                           &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module.");
  }
  return shaderModule;
}

VkShaderModule createShaderModule(VkDevice device, std::string shaderCodePath,
                                  const VkAllocationCallbacks *pAllocator) {
  std::ifstream file(shaderCodePath, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open " + shaderCodePath);
  }
  size_t bufferSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(bufferSize);
  file.seekg(0);
  file.read(buffer.data(), bufferSize);
  file.close();

  return createShaderModule(device, buffer, pAllocator);
}

VkPipelineLayout
createPipelineLayout(VkDevice device,
                     const VkPipelineLayoutCreateInfo *pCreateInfo,
                     const VkAllocationCallbacks *pAllocator) {
  VkPipelineLayout pipelineLayout;
  if (vkCreatePipelineLayout(device, pCreateInfo, pAllocator,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout.");
  }
  return pipelineLayout;
}

VkRenderPass createRenderPass(VkDevice device,
                              const VkRenderPassCreateInfo *pCreateInfo,
                              const VkAllocationCallbacks *pAllocator) {
  VkRenderPass renderPass;
  if (vkCreateRenderPass(device, pCreateInfo, pAllocator, &renderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout.");
  }
  return renderPass;
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