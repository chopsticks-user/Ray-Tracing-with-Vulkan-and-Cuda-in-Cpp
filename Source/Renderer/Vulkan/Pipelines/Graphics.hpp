#ifndef HDG_RENDERER_VULKAN_PIPELINES_GRAPHICS_HPP
#define HDG_RENDERER_VULKAN_PIPELINES_GRAPHICS_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

namespace Neko::Internal::VK
{
    class Device;
    class Swapchain;
    class PipelineLayout;
    class PipelineCache;
    class RenderPass;
    enum class ShaderStage;

    class GraphicsPipeline
    {
    public:
        GraphicsPipeline() = default;
        GraphicsPipeline(const GraphicsPipeline &) = delete;
        GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

        GraphicsPipeline(
            const Core::EngineConfigs &crConfigs, const Device &crDevice,
            const Swapchain &crSwapchain, const RenderPass &crRenderPass,
            const PipelineLayout &crPipelineLayout,
            const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths);

        GraphicsPipeline(GraphicsPipeline &&rhs) noexcept;

        GraphicsPipeline &operator=(GraphicsPipeline &&rhs) noexcept;

        ~GraphicsPipeline() noexcept { release(); }

        const VkPipeline &operator*() const noexcept { return mGPipeline; }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        const RenderPass *mpcRenderPass = nullptr;
        const PipelineLayout *mpcPipelineLayout = nullptr;
        VkPipeline mGPipeline = VK_NULL_HANDLE;
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_PIPELINES_GRAPHICS_HPP */