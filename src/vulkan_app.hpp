#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "resources.hpp"
#include "vkh/vkh.hpp"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif /* GLM_FORCE_RADIANS */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <utility>

class Window {
  typedef GLFWwindow *pGLFWwindow;

public:
  Window() : _window{vkh::createWindow(800, 600)} {};
  Window(int width, int height, const char *title = "Vulkan Application",
         GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr)
      : _window{vkh::createWindow(width, height, title, monitor, share)} {}
  Window(const Window &) = delete;
  Window(Window &&rhs) { _moveDataFrom(std::move(rhs)); }
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Window() {
    if (_isOwner) {
      vkh::destroyWindow(_window);
      std::cout << "Window destructor" << std::endl;
    }
  }
  const pGLFWwindow &ref() const noexcept { return _window; }

private:
  pGLFWwindow _window;
  bool _isOwner = true;

  void _moveDataFrom(Window &&rhs) {
    _window = rhs._window;
    rhs._isOwner = false;
  }
};

class Instance {
public:
  Instance() = default;
  Instance(VkInstanceCreateInfo *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _pAllocator{pAllocator} {
    _instance = vkh::createInstance(pCreateInfo, pAllocator);
  }
  Instance(const Instance &) = delete;
  Instance(Instance &&rhs) { _moveDataFrom(std::move(rhs)); }
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Instance() {
    if (_isOwner) {
      vkh::destroyInstance(_instance, _pAllocator);
      std::cout << "Instance destructor" << std::endl;
    }
  }
  const VkInstance &ref() const noexcept { return _instance; }

private:
  VkInstance _instance;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = true;

  void _moveDataFrom(Instance &&rhs) {
    _instance = rhs._instance;
    _pAllocator = rhs._pAllocator;
    rhs._isOwner = false;
  }
};

class Surface {
public:
  Surface() = default;
  Surface(VkInstance instance, GLFWwindow *window,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _window{window}, _pAllocator{pAllocator} {
    _surface = vkh::createSurface(instance, window, pAllocator);
  }
  Surface(const Surface &) = delete;
  Surface(Surface &&rhs) { _moveDataFrom(std::move(rhs)); }
  Surface &operator=(const Surface &) = delete;
  Surface &operator=(Surface &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Surface() {
    if (_isOwner) {
      vkh::destroySurface(_instance, _surface, _pAllocator);
      std::cout << "Surface destructor" << std::endl;
    }
  }
  const VkSurfaceKHR &ref() const noexcept { return _surface; }

private:
  VkSurfaceKHR _surface;
  VkInstance _instance;
  GLFWwindow *_window;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = true;

  void _moveDataFrom(Surface &&rhs) {
    _surface = rhs._surface;
    _instance = rhs._instance;
    _window = rhs._window;
    _pAllocator = rhs._pAllocator;
    rhs._isOwner = false;
  }
};

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
  Window window;
  void createWindow();

  /* Step 1: Create an instance */
  // VkInstance instance;
  Instance instance;

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
  // VkSurfaceKHR surface;
  Surface surface;

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
  vkh::SwapChainWrapper swapchain;

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