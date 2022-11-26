#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "resources.hpp"
#include "vk_wrappers.hpp"
#include "vkh/vkh.hpp"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif /* GLM_FORCE_RADIANS */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>

class VulkanApp {
public:
  VulkanApp();
  ~VulkanApp();
  VulkanApp(const VulkanApp &) = delete;
  VulkanApp(VulkanApp &&) = delete;
  VulkanApp &operator=(const VulkanApp &) = delete;
  VulkanApp &operator=(VulkanApp &&) = delete;

  void run();

  void writeInfo(std::string filePath);

private:
  const std::vector<const char *> instanceExtensions = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  const std::vector<const char *> instanceLayers = {
      "VK_LAYER_KHRONOS_validation", "VK_LAYER_MANGOHUD_overlay"};
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME};

  /* Step 0: Setup GLFW and window */
  vkw::GLFW glfw = {};
  vkw::Window window = {};

  /* Step 1: Create an instance */
  vkw::Instance instance = {};

  /* Step 2: Setup layers */
  /* Debug messenger of validation layers */
  vkw::DebugMessenger debugMessenger = {instance.ref(), &instance.debugInfo()};

  /* Step 3: Create a window surface */
  vkw::Surface surface = {instance.ref(), window.ref()};

  /* Step 4: Create a logical device */
  /* For graphics, computing, and presentation */
  /* Create a logical device after succesfully selecting a physical device
  and one of its queue families by calling the selectQueueFamily function. */
  vkw::Device device = {instance.ref(), surface.ref()};

  /* Step 5: Create a swapchain to render results to the surface */
  vkh::SwapChainWrapper swapchain;
  // swapchain = {surface.ref(), device.ref(), device.physical()};

  const VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  // const VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;

  const uint32_t swapchainCount = 1;

  /* Included when selecting a physical device */

  VkSwapchainCreateInfoKHR populateSwapchainCreateInfo();

  void createSwapchain();

  /* Step 6: Create image views */
  std::vector<VkImageView> imageViews;

  /* Presentable images. Only one image is displayed at a time,
  the others are queued for presentation. A presentable image  must be
  used after the image is returned by vkAccquireNextImageKHR and before
  it is released by vkQueuePresentKHR. */
  std::vector<VkImage> images;

  void createImageViews();

  /* Step 7: Create a graphics pipeline */
  vkw::GraphicsPipeline graphicsPipeline;
  // vkh::GraphicsPipelineDepWrapper graphicsPipelineDeps;

  const uint32_t graphicsPipelineCount = 1;

  void createGraphicsPipeline();

  /* Step 8: Create framebuffers */
  vkw::Framebuffers framebuffers;

  void createFramebuffers();

  /* Step 9: Recreate swapchain */
  void recreateSwapchain();

  void cleanupSwapchain();

  bool framebufferResized = false;

  static void framebufferResizeCallback(GLFWwindow *windowInstance, int width,
                                        int height);

  /* Step 10: Command buffers */

  /* Affects the number of command buffers, semaphores, and fences */
  const size_t maxFramesInFlight = 2;

  std::vector<VkCommandBuffer> commandBuffers;

  // std::vector<VkCommandBuffer> commandBuffers;

  /**
   * @brief Command pools are externally synchronized, meaning that a command
   * pool must not be used concurrently in multiple threads. That includes use
   * via recording commands on any command buffers allocated from the pool, as
   * well as operations that allocate, free, and reset command buffers or the
   * pool itself.
   */
  VkCommandPool commandPool;

  void createCommandPool();

  void createCommandBuffers();

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  /* Step 11: Synchronization and cache control */
  struct SyncWrapper {
    std::vector<VkSemaphore> imageAvailableSemaphore;
    std::vector<VkSemaphore> renderFinisedSemaphore;
    std::vector<VkFence> inFlightFence;
    size_t currentFrame = 0;
  } sync;

  void createSynchronizationObjects();

  /* Step 12: Buffers */
  /* Before {createCommandBuffers()} and after {createCommandPool()} */
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;

  /* Query memory requirements */
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propFlags);

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags property, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);

  void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

  void createVertexBuffer();

  void createIndexBuffer();

  void createUniformBuffers();

  void updateUniformBuffer(uint32_t imageIndex);

  /* Step 13: Resource descriptors */
  /* Usage of descriptors consists of three parts:
  • Specify a descriptor layout during pipeline creation
  • Allocate a descriptor set from a descriptor pool
  • Bind the descriptor set during rendering */

  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  void createDescriptorSetLayout();

  void createDescriptorPool();

  void createDescriptorSets();

  /* Step 14: Textures */

  void createTextureImage();

  /* Last step: Render */
  void render();
};
#endif /* VULKAN_APP_HPP */