#include "PipelineCache.hpp"

#include "Devices/LogicalDevice.hpp"

namespace Neko
{

    PipelineCache::PipelineCache(const Device &crDevice, u64 initialDataSize,
                                 const void *pcInititalData,
                                 VkPipelineCacheCreateFlags flags)
        : mpcDevice{&crDevice}
    {
        VkPipelineCacheCreateInfo pipelineCacheInfo{};
        pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        pipelineCacheInfo.pNext = nullptr;
        pipelineCacheInfo.flags = flags;
        pipelineCacheInfo.initialDataSize = initialDataSize;
        pipelineCacheInfo.pInitialData = pcInititalData;
        if (vkCreatePipelineCache(*crDevice, &pipelineCacheInfo, nullptr, &mPCache) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline cache");
        }
    }

    PipelineCache::PipelineCache(PipelineCache &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)}, mPCache{std::exchange(
                                                   rhs.mPCache, VK_NULL_HANDLE)} {}

    PipelineCache &PipelineCache::operator=(PipelineCache &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mPCache = std::exchange(rhs.mPCache, VK_NULL_HANDLE);
        return *this;
    }

    void PipelineCache::release() noexcept
    {
        if (mPCache != VK_NULL_HANDLE)
        {
            vkDestroyPipelineCache(**mpcDevice, mPCache, nullptr);
            mPCache = VK_NULL_HANDLE;
        }
    }

} // namespace Neko