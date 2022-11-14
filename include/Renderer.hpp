#ifndef RTVC_RENDERER_HPP
#define RTVC_RENDERER_HPP

#include "ProxyFuncs.hpp"
#include "Utility.hpp"
#include "Vertex.hpp"
#include "triangle.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace rtvc {
class RenderApplication {
public:
  static const uint32_t init_window_width = 1024;
  static const uint32_t init_window_height = 720;
  static const uint32_t max_frames_in_flight = 2;
#ifdef NDEBUG
  static const bool enableValidationLayers = false;
#else
  static const bool enableValidationLayers = true;
#endif /* NDEBUG */

  // RenderApplication()
  //     : window{nullptr, init_window_width, init_window_height} {}
  // RenderApplication(uint32_t w, uint32_t h) : window{nullptr, w, h} {}

  RenderApplication()
      : window{nullptr}, width{init_window_width}, height{init_window_height} {}
  RenderApplication(uint32_t w, uint32_t h)
      : window{nullptr}, width{w}, height{h} {}

  void run();

private:
  // struct Window {
  //   GLFWwindow *ref;
  //   uint32_t width;
  //   uint32_t height;
  // } window;

  GLFWwindow *window;
  uint32_t width;
  uint32_t height;

  VkInstance instance;

  const std::vector<const char *> validationLayers = {
      "VK_LAYER_KHRONOS_validation"};

  VkDebugUtilsMessengerEXT debugMessenger;

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  VkDevice device;

  VkQueue graphicsQueue;

  VkSurfaceKHR surface;

  VkQueue presentQueue;

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImageView> swapChainImageViews;

  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

  std::vector<VkFramebuffer> swapChainFrameBuffers;

  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  bool frameBufferResized = false;

  uint32_t currentFrame = 0;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT,
      const VkDebugUtilsMessengerCallbackDataEXT *, void *);

  bool checkValidationLayerSupport();

  std::vector<VkExtensionProperties> getSupportedExtensions();

  std::vector<const char *> getRequiredExtensions();

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &);

  void setupDebugMessenger();

  void creatInstance();

  bool checkDeviceExtensionSupport(VkPhysicalDevice);

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);

  bool isSuitableDevice(VkPhysicalDevice);

  int rateDeviceSuitability(VkPhysicalDevice);

  void selectPhysicalDevice();

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);

  void createLogicalDevice();

  void createSurface();

  VkSurfaceFormatKHR
  selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &);

  VkPresentModeKHR selectSwapPresentMode(const std::vector<VkPresentModeKHR> &);

  VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR &);

  void createSwapChain();

  void createImageViews();

  std::vector<char> readShader(const std::string &);

  VkShaderModule createShaderModule(const std::vector<char> &);

  void createRenderPass();

  void createGraphicsPipeline();

  void createFramebuffers();

  void createCommandPool();

  void createBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags,
                    VkBuffer &, VkDeviceMemory &);

  void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);

  void createVertexBuffers();

  void createCommandBuffers();

  void recordCommandBuffer(VkCommandBuffer, uint32_t);

  void createSyncObjects();

  void drawFrame();

  void cleanUpSwapChain();

  void recreateSwapChain();

  static void framebufferResizeCallback(GLFWwindow *, int, int);

  uint32_t findMemoryType(uint32_t, VkMemoryPropertyFlags);

  void initWindow();

  void initVulkan();

  void mainLoop();

  void cleanUp();
};
} // namespace rtvc

#endif // RTVC_RENDERER_HPP