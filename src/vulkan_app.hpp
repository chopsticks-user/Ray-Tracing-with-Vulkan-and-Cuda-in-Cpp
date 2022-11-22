#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "vkh/vkh.hpp"

#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <utility>

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
  /* Step 0: Setup GLFW and window */
  GLFWwindow *window;
  void createWindow();

  /* Step 1: Create an instance */
  VkInstance instance;

  std::vector<const char *> instanceExtensions = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

  void createInstance();

  /* Step 2: Setup layers */
  const std::vector<const char *> instanceLayers = {
      "VK_LAYER_KHRONOS_validation", "VK_LAYER_MANGOHUD_overlay"};

  /* Debug messenger of validation layers */
  VkDebugUtilsMessengerEXT debugMessenger;

  void createDebugMessenger();

  /* Step 3: Create a window surface */
  VkSurfaceKHR surface;

  void createSurface();

  /* Step 4: Create a logical device */
  VkDevice device;
  VkPhysicalDevice physicalDevice;

  /* For graphics, computing, and presentation */
  VkQueue queue;
  uint32_t queueFamilyIndex;

  bool checkDeviceProperties(VkPhysicalDevice physDev);

  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
  selectQueueFamily(VkPhysicalDevice physicalDevice);

  /* Create a logical device after succesfully selecting a physical device
  and one of its queue families by calling the selectQueueFamily function. */
  void createDevice();

  /* Step 5: Create a swapchain to render results to the surface */
  struct SwapChainWrapper {

    /* A swapchain instance */
    VkSwapchainKHR self;

    VkFormat format;

    VkExtent2D extent;

  } swapchain;

  const VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  // const VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;

  const uint32_t swapchainCount = 1;

  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME};

  /* Included when selecting a physical device */
  bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
  bool checkDeviceSwapchainSupport(VkPhysicalDevice physicalDevice);

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
  vkh::GraphicsPipelineWrapper graphicsPipeline;

  const uint32_t graphicsPipelineCount = 1;

  void createGraphicsPipeline();

  /* Step 8: Create framebuffers */
  std::vector<VkFramebuffer> framebuffers;

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

  void createCommandBuffer();

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  /* Step 11: Synchronization and cache control */
  struct SyncWrapper {
    std::vector<VkSemaphore> imageAvailableSemaphore;
    std::vector<VkSemaphore> renderFinisedSemaphore;
    std::vector<VkFence> inFlightFence;
    size_t currentFrame = 0;
  } sync;

  void createSynchronizationObjects();

  /* Step : Render */
  void render();
};
#endif /* VULKAN_APP_HPP */