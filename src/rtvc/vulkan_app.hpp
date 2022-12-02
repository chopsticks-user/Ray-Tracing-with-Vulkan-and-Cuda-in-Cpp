#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "rtvc/buffer.hpp"
#include "rtvc/command_pool.hpp"
#include "rtvc/descriptor.hpp"
#include "rtvc/device.hpp"
#include "rtvc/framebuffers.hpp"
#include "rtvc/graphics_pipeline.hpp"
#include "rtvc/image_view.hpp"
#include "rtvc/instance.hpp"
// #include "rtvc/resources.hpp"
#include "rtvc/compute_pipeline.hpp"
#include "rtvc/ray_tracing_pipeline.hpp"
#include "rtvc/sampler.hpp"
#include "rtvc/swapchain.hpp"
#include "rtvc/sync.hpp"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif /* GLM_FORCE_RADIANS */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>
#include <unordered_map>

namespace rtvc {

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
  const std::string imagePath =
      absoluteDirectory + "/resources/textures/texture.jpeg";

  const std::string modelObjectPath =
      absoluteDirectory + "/resources/models/viking_room.obj";
  const std::string modelTexturePath =
      absoluteDirectory + "/resources/models/viking_room.png";

  /* Setup GLFW and window */
  GLFW glfw = {};
  Window window = {};

  /* Create an instance */
  Instance instance = {};

  /* Setup layers */
  /* Debug messenger of validation layers */
  DebugMessenger debugMessenger = {instance};

  /* Create a window surface */
  Surface surface = {instance, window};

  /* Create a logical device */
  /* For graphics, computing, and presentation */
  /* Create a logical device after succesfully selecting a physical device
  and one of its queue families by calling the selectQueueFamily function. */
  Device device = {instance, surface};

  /* Create a swapchain to render results to the surface */
  Swapchain swapchain = {surface, device, preferredPresentMode};

  /* Included when selecting a physical device */

  /* Create image views */
  /* Only one image is displayed at a time, the others are queued for
  presentation. A presentable image  must be used after the image is
  returned by {vkAccquireNextImageKHR} and before it is released by
  {vkQueuePresentKHR} */
  SwapchainImageViews imageViews = {device, swapchain};

  DescriptorSetLayout descriptorSetLayout = {device};

  /* Command buffers */
  /**
   * @brief Command pools are externally synchronized, meaning that a command
   * pool must not be used concurrently in multiple threads. That includes use
   * via recording commands on any command buffers allocated from the pool, as
   * well as operations that allocate, free, and reset command buffers or the
   * pool itself.
   */
  CommandPool commandPool = {device};

  CommandBuffers commandBuffers = {
      commandPool.allocateBuffers(maxFramesInFlight)};

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  /* MSAA */

  VkSampleCountFlagBits getMaxSampleCount();

  bool enableMSAA = true;
  VkSampleCountFlagBits msaaSamples = getMaxSampleCount();

  Image colorImage = {device,
                      {
                          swapchain.extent().width,
                          swapchain.extent().height,
                          1,
                          msaaSamples,
                          swapchain.format(),
                          VK_IMAGE_TILING_OPTIMAL,
                          VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                              VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      }};
  ImageView colorImageView = {
      device, colorImage, swapchain.format(), VK_IMAGE_ASPECT_COLOR_BIT, 1,
  };

  /* Depth resources */
  /* A depth attachment is based on an image, just like the color attachment.
  The difference is that the swap chain will not automatically create depth
  images for us. We only need a single depth image, because only one draw
  operation is running at once. */

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat findDepthFormat();
  bool hasStencilComponent(VkFormat format);

  Image makeDepthImage(VkFormat format);
  ImageView makeDepthView(const Image &image, VkFormat format);

  VkFormat depthFormat = findDepthFormat();
  Image depthImage = makeDepthImage(depthFormat);
  ImageView depthImageView = makeDepthView(depthImage, depthFormat);

  /* Create a graphics pipeline */
  GraphicsPipeline graphicsPipeline = {{
      device,
      swapchain,
      depthFormat,
      msaaSamples,
      descriptorSetLayout,
      "/build/shaders/triangle_vert.spv",
      "/build/shaders/triangle_frag.spv",
  }};

  /* Create framebuffers */
  Framebuffers framebuffers = {
      device,         imageViews,       depthImageView,
      colorImageView, graphicsPipeline, swapchain,
  };

  /* Recreate swapchain */
  void recreateSwapchain();
  static void framebufferResizeCallback(GLFWwindow *windowInstance, int width,
                                        int height);
  bool framebufferResized = false;

  /* Synchronization and cache control */
  SyncWrapper sync = {device, maxFramesInFlight};

  /* Buffers */

  /* Models */
  struct ModelWrapper {
    std::vector<vkw::Vertex> vertices;
    std::vector<uint32_t> indices;
  };

  ModelWrapper loadModel();

  /* Query memory requirements */
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propFlags);

  vkw::Buffer makeVertexBuffer();
  vkw::Buffer makeIndexBuffer();
  std::vector<vkw::Buffer> makeUniformBuffers();
  vkw::UniformBufferObject updateUBO(float elapsedTime);

  //   std::vector<vkw::Vertex> vertices;
  //   std::vector<uint32_t> indices;
  ModelWrapper model = loadModel();
  vkw::Buffer vertexBuffer = makeVertexBuffer();
  vkw::Buffer indexBuffer = makeIndexBuffer();
  std::vector<vkw::Buffer> uniformBuffers = makeUniformBuffers();

  /* Textures images */

  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout,
                             uint32_t mipLevels = 1);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);
  void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t width,
                       int32_t height, uint32_t mipLevels);

  Image makeTextureImage();

  uint32_t _mipLevels = 1;
  Image textureImage = makeTextureImage();
  ImageView textureImageView = {
      device,
      textureImage,
      VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_ASPECT_COLOR_BIT,
      _mipLevels,
  };
  Sampler textureSampler = {device, _mipLevels};

  /* Resource descriptors */
  /* Usage of descriptors consists of three parts:
  • Specify a descriptor layout during pipeline creation
  • Allocate a descriptor set from a descriptor pool
  • Bind the descriptor set during rendering */

  vkw::DescriptorSets makeDescriptorSets();

  DescriptorPool descriptorPool = {device,
                                   static_cast<uint32_t>(maxFramesInFlight)};
  vkw::DescriptorSets descriptorSets = makeDescriptorSets();

  /* Handle events and render */

  void getInputEvents();

  /* Scroll: zoom */
  static void scrollEventCallback(GLFWwindow *pWindow, double xOffset,
                                  double yOffset);
  const float minScale = 0.5f;
  const float maxScale = 2.0f;
  const float incScale = 0.1f;
  float currentScale = 1.0f;

  /* Keyboard events */
  static void keyboardEventCallback(GLFWwindow *pWindow, int key, int scancode,
                                    int action, int mods);
  bool paused = false;
  float currentAngle = 0.0f;
  const float angleVelocity = 45.0f;

  void updateFrame(uint32_t imageIndex);

  void render();
};

} /* namespace rtvc */
#endif /* VULKAN_APP_HPP */