#ifndef NEKO_RENDERER_VULKAN_BASIC_SWAPCHAIN_HPP
#define NEKO_RENDERER_VULKAN_BASIC_SWAPCHAIN_HPP

#include "CoreInternal.hpp"
#include "Context.hpp"

namespace Neko::Internal::VK
{

    class Surface;
    class Device;

    class Swapchain
    {
    public:
        Swapchain() = default;
        Swapchain(const Swapchain &) = delete;
        Swapchain &operator=(const Swapchain &) = delete;

        Swapchain(const Core::EngineConfigs &crSettings, const Surface &crSurface,
                  const Device &crDevice);

        Swapchain(Swapchain &&rhs) noexcept;

        Swapchain &operator=(Swapchain &&rhs) noexcept;

        ~Swapchain() noexcept { release(); }

        const VkSwapchainKHR &operator*() const noexcept { return mSwapchain; }

        operator VkSwapchainKHR() const noexcept { return mSwapchain; }

        const VkFormat &format() const noexcept { return mFormat; }

        const VkExtent2D &extent() const noexcept { return mExtent; }

        std::vector<VkImage> getImages() const;

        const std::vector<VkImageView> &getImageViews() const noexcept
        {
            return mImageViews;
        }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
        VkFormat mFormat = {};
        VkExtent2D mExtent = {};
        std::vector<VkImageView> mImageViews = {};

        void initializeImageViews();
    };

} // namespace Neko::Internal::VK

#endif /* NEKO_RENDERER_VULKAN_BASIC_SWAPCHAIN_HPP */