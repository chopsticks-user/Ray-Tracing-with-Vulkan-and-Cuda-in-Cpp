#ifndef NEKO_RENDERER_VULKAN_BASIC_SURFACE_HPP
#define NEKO_RENDERER_VULKAN_BASIC_SURFACE_HPP

#include "CoreInternal.hpp"
#include "Context.hpp"

namespace Neko::Internal::VK
{
    class Instance;
    class Window;

    class Surface
    {
    public:
        Surface() = default;

        Surface(const Instance &crInstance, const Window &crWindow);

        Surface(const Surface &) = delete;

        Surface(Surface &&) noexcept;

        Surface &operator=(const Surface &) = delete;

        Surface &operator=(Surface &&) noexcept;

        ~Surface() noexcept { release(); }

        const VkSurfaceKHR &operator*() const noexcept { return mSurface; }

        void release() noexcept;

    private:
        const Instance *mpcInstance = nullptr;
        VkSurfaceKHR mSurface = VK_NULL_HANDLE;
    };

} // namespace Neko::Internal::VK

#endif /* NEKO_RENDERER_VULKAN_BASIC_SURFACE_HPP */