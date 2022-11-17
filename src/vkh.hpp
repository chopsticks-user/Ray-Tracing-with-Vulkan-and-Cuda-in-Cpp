#ifndef VKH_HPP
#define VKH_HPP

#include "create_object.hpp"
#include "destroy_object.hpp"
#include "get_list.hpp"
#include "make_info.hpp"
#include "utility.hpp"
#include "validation_layers.hpp"

#include <optional>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> computeFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && computeFamily.has_value();
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

private:
  /* Step 0: Setup GLFW and window */

  GLFWwindow *window;
  void createWindow();

  /* Step 1: Create an instance */

  VkInstance instance;
  void createInstance();

  /* Step 2: Setup validation layers */

  const std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};
  VkDebugUtilsMessengerEXT debugMessenger;

  void createDebugMessenger();

  /* Step 3: Create a window surface */

  VkSurfaceKHR surface;

  void createSurface();

  /* Step 4: Create a logical device */

  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue computeQueue;

  void selectQueueFamilies();
  void queryPhysicalDeviceProperties();
  void selectPhysicalDevice();
  void createDevice();

  /* Step 5: Create a swap chain */
};

#endif /* VKH_HPP */
