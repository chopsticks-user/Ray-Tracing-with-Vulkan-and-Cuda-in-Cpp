#ifndef NEKO_RENDERER_PIPELINES_RENDER_PASS_HPP
#define NEKO_RENDERER_PIPELINES_RENDER_PASS_HPP

#include "CoreNeko.hpp"

namespace Neko
{

    class Device;
    class Swapchain;
    class DepthBuffer;

    class RenderPass
    {
    public:
        RenderPass() = default;
        RenderPass(const RenderPass &) = delete;
        RenderPass &operator=(const RenderPass &) = delete;

        RenderPass(const Device &crDevice,
                   const VkRenderPassCreateInfo &crRenderPassInfo);

        RenderPass(const Device &crDevice, const Swapchain &crSwapchain,
                   const DepthBuffer &crDepthBuffer);

        RenderPass(RenderPass &&rhs) noexcept;

        RenderPass &operator=(RenderPass &&rhs) noexcept;

        ~RenderPass() noexcept { release(); }

        const VkRenderPass &operator*() const noexcept { return mRenderPass; }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkRenderPass mRenderPass = VK_NULL_HANDLE;
    };

} // namespace Neko

#endif /* NEKO_RENDERER_PIPELINES_RENDER_PASS_HPP */