#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "resources.hpp"

#include <vkw.hpp>

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
  ~VulkanApp() = default;
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
  const VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

  /* Affects the number of command buffers, semaphores, and fences */
  const size_t maxFramesInFlight = 2;
  const uint32_t swapchainCount = 1;

  /* Image path used in {createTextureImage()} */
  const std::string imagePath = "";

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
  vkw::Swapchain swapchain = {surface.ref(), device.ref(), device.physical(),
                              preferredPresentMode};

  /* Included when selecting a physical device */

  /* Step 6: Create image views */
  /* Only one image is displayed at a time, the others are queued for
  presentation. A presentable image  must be used after the image is
  returned by {vkAccquireNextImageKHR} and before it is released by
  {vkQueuePresentKHR} */
  vkw::ImageViews imageViews = {device.ref(), swapchain.ref(),
                                swapchain.format()};

  vkw::DescriptorSetLayout descriptorSetLayout = {device.ref()};

  /* Step 7: Create a graphics pipeline */
  vkw::GraphicsPipeline graphicsPipeline = {device.ref(),
                                            swapchain.extent(),
                                            swapchain.format(),
                                            &descriptorSetLayout.ref(),
                                            "/build/shaders/triangle_vert.spv",
                                            "/build/shaders/triangle_frag.spv"};

  /* Step 8: Create framebuffers */
  vkw::Framebuffers framebuffers = {device.ref(), imageViews.ref(),
                                    graphicsPipeline.renderPass(),
                                    swapchain.extent()};

  /* Step 9: Recreate swapchain */
  void recreateSwapchain();
  static void framebufferResizeCallback(GLFWwindow *windowInstance, int width,
                                        int height);
  bool framebufferResized = false;

  /* Step 10: Command buffers */
  /**
   * @brief Command pools are externally synchronized, meaning that a command
   * pool must not be used concurrently in multiple threads. That includes use
   * via recording commands on any command buffers allocated from the pool, as
   * well as operations that allocate, free, and reset command buffers or the
   * pool itself.
   */
  vkw::CommandPool commandPool = {device.ref(), device.familyIndex()};

  std::vector<VkCommandBuffer> commandBuffers = {
      commandPool.allocateBuffers(maxFramesInFlight)};

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  /* Step 11: Synchronization and cache control */
  struct SyncWrapper {
    std::vector<vkw::Semaphore> imageAvailableSemaphore;
    std::vector<vkw::Semaphore> renderFinishedSemaphore;
    std::vector<vkw::Fence> inFlightFence;
    size_t currentFrame = 0;

    SyncWrapper(VkDevice device, size_t frameCount)
        : imageAvailableSemaphore{frameCount},
          renderFinishedSemaphore{frameCount}, inFlightFence{frameCount} {
      for (size_t i = 0; i < frameCount; ++i) {
        imageAvailableSemaphore[i] = {device};
        renderFinishedSemaphore[i] = {device};
        inFlightFence[i] = {device, VK_FENCE_CREATE_SIGNALED_BIT};
      }
    }
  } sync = {device.ref(), maxFramesInFlight};

  /* Step 12: Buffers */

  /* Query memory requirements */
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propFlags);
  vkw::Buffer makeVertexBuffer();
  vkw::Buffer makeIndexBuffer();
  std::vector<vkw::Buffer> makeUniformBuffers();
  void updateUniformBuffer(uint32_t imageIndex);

  vkw::Buffer vertexBuffer = makeVertexBuffer();
  vkw::Buffer indexBuffer = makeIndexBuffer();
  std::vector<vkw::Buffer> uniformBuffers = makeUniformBuffers();

  /* Step 13: Resource descriptors */
  /* Usage of descriptors consists of three parts:
  • Specify a descriptor layout during pipeline creation
  • Allocate a descriptor set from a descriptor pool
  • Bind the descriptor set during rendering */
  vkw::DescriptorPool descriptorPool = {
      device.ref(), static_cast<uint32_t>(maxFramesInFlight)};

  vkw::DescriptorSets makeDescriptorSets();
  vkw::DescriptorSets descriptorSets = makeDescriptorSets();

  /* Step 14: Textures images */
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;

  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags propFlags, VkImage &image,
                   VkDeviceMemory &imageMemory);
  void createTextureImage();

  /* Last step: Render */
  void render();
};
#endif /* VULKAN_APP_HPP */