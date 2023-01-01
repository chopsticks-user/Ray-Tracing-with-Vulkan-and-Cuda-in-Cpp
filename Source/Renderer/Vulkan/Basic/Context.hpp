#ifndef HDG_RENDERER_VULKAN_BASIC_CONTEXT_HPP
#define HDG_RENDERER_VULKAN_BASIC_CONTEXT_HPP

#include "CoreInternal.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // GLFW_INCLUDE_VULKAN

namespace Neko::Internal::VK
{
    class Context
    {
    public:
        Context() { createContext(); }

        Context(const Context &) = delete;

        Context(Context &&) noexcept { moveCreateContext(); }

        Context &operator=(const Context &) = delete;

        Context &operator=(Context &&) = default;

        ~Context() { destroyContext(); }

        VkResult
        createDebugMessenger(VkInstance instance,
                             const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                             const VkAllocationCallbacks *pAllocator,
                             VkDebugUtilsMessengerEXT *pMessenger) const;

        void
        destroyDebugMessenger(VkInstance instance,
                              VkDebugUtilsMessengerEXT debugMessenger,
                              const VkAllocationCallbacks *pAllocator) const noexcept;

    private:
        static void createContext();

        static void moveCreateContext();

        static void destroyContext();
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_BASIC_CONTEXT_HPP */