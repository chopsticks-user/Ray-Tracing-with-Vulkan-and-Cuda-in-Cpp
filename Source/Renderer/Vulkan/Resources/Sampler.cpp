#include "Sampler.hpp"

#include "Devices/LogicalDevice.hpp"

namespace Neko
{

    Sampler::Sampler([[maybe_unused]] const EngineConfigs &crConfig,
                     const Device &crDevice, u32 mipLevels)
        : mpcDevice{&crDevice}
    {
        [[maybe_unused]] VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(crDevice.physical(), &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        // samplerInfo.anisotropyEnable = VK_TRUE;
        // samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.maxLod = static_cast<float>(mipLevels);
        samplerInfo.minLod = 0;
        if (vkCreateSampler(*crDevice, &samplerInfo, nullptr, &mSampler) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create sampler");
        }
    }

    Sampler::Sampler(Sampler &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)}, mSampler{std::exchange(
                                                   rhs.mSampler, VK_NULL_HANDLE)} {}

    Sampler &Sampler::operator=(Sampler &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mSampler = std::exchange(rhs.mSampler, VK_NULL_HANDLE);
        return *this;
    }

    void Sampler::release() noexcept
    {
        if (mSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(**mpcDevice, mSampler, nullptr);
            mSampler = VK_NULL_HANDLE;
        }
    }

} // namespace Neko