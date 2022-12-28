#include "PipelineLayout.hpp"

#include "Devices/Logical.hpp"
#include "Resources/Descriptors.hpp"

namespace Neko::Internal::VK
{

    PipelineLayout::PipelineLayout(
        const Device &crDevice,
        const std::vector<VkDescriptorSetLayout> &crDSLayout)
        : mpcDevice{&crDevice}
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = vku32(crDSLayout.size());
        pipelineLayoutInfo.pSetLayouts = crDSLayout.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(*crDevice, &pipelineLayoutInfo, nullptr,
                                   &mPLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    PipelineLayout::PipelineLayout(PipelineLayout &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)}, mPLayout{std::exchange(
                                                   rhs.mPLayout, VK_NULL_HANDLE)} {}

    PipelineLayout &PipelineLayout::operator=(PipelineLayout &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mPLayout = std::exchange(rhs.mPLayout, VK_NULL_HANDLE);
        return *this;
    }

    void PipelineLayout::release() noexcept
    {
        if (mPLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(**mpcDevice, mPLayout, nullptr);
            mPLayout = VK_NULL_HANDLE;
        }
    }

} // namespace Neko::Internal::VK