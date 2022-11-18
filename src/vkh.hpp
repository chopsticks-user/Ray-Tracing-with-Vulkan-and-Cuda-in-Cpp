#ifndef VKH_HPP
#define VKH_HPP

#include "create_object.hpp"
#include "destroy_object.hpp"
#include "get_list.hpp"
#include "make_info.hpp"
#include "utility.hpp"
#include "validation_layers.hpp"

#include <optional>
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

private:
  /* Step 0: Setup GLFW and window */
  GLFWwindow *window;
  void createWindow();

  /* Step 1: Create an instance */
  VkInstance instance;
  void createInstance();

  /* Step 2: Setup layers */
  const std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};

  /* Debug messenger of validation layers */
  VkDebugUtilsMessengerEXT debugMessenger;

  void createDebugMessenger();

  /* Step 3: Create a window surface */
  VkSurfaceKHR surface;

  void createSurface();

  /* Step 4: Create a logical device */
  VkDevice device;

  /* For graphics, computing, and presentation */
  VkQueue queue;

  std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
  selectQueueFamily(VkPhysicalDevice physicalDevice);

  /* Create a logical device after succesfully selecting a physical device
  and one of its queue families by calling the selectQueueFamily function */
  void createDevice();

  /* Step 5: Create a swapchain */
  VkSwapchainKHR swapchain;

  void createSwapchian();
};

#endif /* VKH_HPP */
