#ifndef HDG_RENDERER_VULKAN_PIPELINES_LAYOUT_HPP
#define HDG_RENDERER_VULKAN_PIPELINES_LAYOUT_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

namespace Neko::Internal::VK
{
    class Device;
    class DescriptorSetLayout;

    class PipelineLayout
    {
    public:
        PipelineLayout() = default;
        PipelineLayout(const PipelineLayout &) = delete;
        PipelineLayout &operator=(const PipelineLayout &) = delete;

        PipelineLayout(const Device &crDevice,
                       const std::vector<VkDescriptorSetLayout> &crDSLayout);

        PipelineLayout(PipelineLayout &&rhs) noexcept;

        PipelineLayout &operator=(PipelineLayout &&rhs) noexcept;

        ~PipelineLayout() noexcept { release(); }

        const VkPipelineLayout &operator*() const noexcept { return mPLayout; }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkPipelineLayout mPLayout = VK_NULL_HANDLE;
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_PIPELINES_LAYOUT_HPP */