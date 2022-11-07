#ifndef RTVULKAN_RENDERER_HPP
#define RTVULKAN_RENDERER_HPP

#include "ProxyFuncs.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>

namespace rt_vulkan
{
    class RenderTriangleApplication
    {
    public:
        static const uint32_t init_window_width = 1024;
        static const uint32_t init_window_height = 720;

        RenderTriangleApplication()
            : window{nullptr, init_window_width, init_window_height} {}
        RenderTriangleApplication(uint32_t w, uint32_t h)
            : window{nullptr, w, h} {}

        void run();

    private:
        struct Window
        {
            GLFWwindow *ref;
            uint32_t width;
            uint32_t height;
        } window;

        VkInstance instance;

        const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"};
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif /* NDEBUG */
        VkDebugUtilsMessengerEXT debugMessenger;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete()
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        VkDevice device;

        VkQueue graphicsQueue;

        VkSurfaceKHR surface;

        VkQueue presentQueue;

        struct SwapChainSupportDetails
        {
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

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT,
            VkDebugUtilsMessageTypeFlagsEXT,
            const VkDebugUtilsMessengerCallbackDataEXT *,
            void *);

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
        VkSurfaceFormatKHR selectSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR> &);
        VkPresentModeKHR selectSwapPresentMode(
            const std::vector<VkPresentModeKHR> &);
        VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR &);
        void createSwapChain();
        void createImageView();
        std::vector<char> readShader(const std::string &);
        VkShaderModule createShaderModule(const std::vector<char> &);
        void createGraphicsPipeline();

        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanUp();
    };
} // namespace rt_vulkan

#endif // RTVULKAN_RENDERER_HPP