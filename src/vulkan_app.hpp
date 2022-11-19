#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include "vkh/vkh.hpp"

#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <utility>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics;
  std::optional<uint32_t> compute;
  std::optional<uint32_t> present;

  bool isComplete() { return graphics.has_value() && compute.has_value(); }
};

struct Queues {
  VkQueue graphics;
  VkQueue compute;
  VkQueue present;
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
  GLFWwindow *window;
  void createWindow();

  /* Step 1: Create an instance */
  VkInstance instance;

  std::vector<const char *> instanceExtensions = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

  void createInstance();

  /* Step 2: Setup layers */
  const std::vector<const char *> instanceLayers = {
      "VK_LAYER_KHRONOS_validation"};

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

    /* Presentable images. Only one image is displayed at a time,
    the others are queued for presentation. A presentable image  must be
    used after the image is returned by vkAccquireNextImageKHR and before
    it is released by vkQueuePresentKHR. */
    std::vector<VkImage> images;

    VkFormat format;

    VkExtent2D extent;
  } swapchain;

  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  /* Included when selecting a physical device */
  bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
  bool checkDeviceSwapchainSupport(VkPhysicalDevice physicalDevice);

  VkSwapchainCreateInfoKHR populateSwapchainCreateInfo();

  void createSwapchain();

  /* Step 6:  */
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