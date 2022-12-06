#ifndef RTVC_VULKAN_APP_HPP
#define RTVC_VULKAN_APP_HPP

#include "basic/command_pool.hpp"
#include "basic/device.hpp"
#include "basic/instance.hpp"
#include "basic/swapchain.hpp"

namespace rtvc {

class VulkanApp {
public:
  VulkanApp() = default;
  ~VulkanApp() = default;
  VulkanApp(const VulkanApp &) = delete;
  VulkanApp(VulkanApp &&) = delete;
  VulkanApp &operator=(const VulkanApp &) = delete;
  VulkanApp &operator=(VulkanApp &&) = delete;

  void render() {
    while (!glfwWindowShouldClose(*_window)) {
      glfwPollEvents();
    }
  }

private:
  Settings _settings = {};
  vk::raii::Context _context = {};
  rtvc::GLFW _glfw = {};
  rtvc::Window _window = {};

  vk::raii::Instance _instance = makeInstance(_context);

  vk::raii::DebugUtilsMessengerEXT _debugMessenger =
      makeDebugMessenger(_instance);

  vk::raii::SurfaceKHR _surface = makeSurface(_instance, _window);

  DeviceWrapper _device = makeLogicalDevice(_instance, _surface);

  vk::raii::CommandPool _commandPool = makeCommandPool(_device);

  vk::raii::CommandBuffers _commandBuffers = makeCommandBuffers(
      _device.logical, _commandPool, _settings.maxFramesInFlight);

  SwapchainWrapper _swapchain = makeSwapchain(_settings, _surface, _device);
};

} /* namespace rtvc */
#endif /* RTVC_VULKAN_APP_HPP */