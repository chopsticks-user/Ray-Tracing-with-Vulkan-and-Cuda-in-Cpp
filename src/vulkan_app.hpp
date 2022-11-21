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

  /* Step 5: Create a swapchain to render results to the surface
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount,
                          swapChainImages.data()); */
  struct SwapChainWrapper {

    /* A swapchain instance */
    VkSwapchainKHR self;

    VkFormat format;

    VkExtent2D extent;

  } swapchain;

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

  void createGraphicsPipeline();

  /* Step 8: Create framebuffers */
  std::vector<VkFramebuffer> framebuffers;

  void createFramebuffer();

  /* Step 9: Command buffers */
  VkCommandBuffer commandBuffer;

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
};

// class Instance {
// public:
//   Instance(const VkInstanceCreateInfo *pCreateInfo,
//            const VkAllocationCallbacks *pAllocator = nullptr) {
//     if (vkCreateInstance(pCreateInfo, pAllocator, &ref) != VK_SUCCESS) {
//       throw std::runtime_error("Failed to create an instance.");
//     }
//   }

//   ~Instance() { vkDestroyInstance(ref, pAlloc); }

//   Instance() = delete;
//   Instance(const Instance &) = delete;
//   Instance(Instance &&) = delete;
//   Instance &operator=(const Instance &) = delete;
//   Instance &operator=(Instance &&) = delete;

// private:
//   VkInstance ref;
//   const VkAllocationCallbacks *pAlloc;
// };

#endif /* VULKAN_APP_HPP */