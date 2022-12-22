#ifndef NEKO_RENDERER_PIPELINES_CACHE_HPP
#define NEKO_RENDERER_PIPELINES_CACHE_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;

class PipelineCache {
public:
  PipelineCache() = default;
  PipelineCache(const PipelineCache &) = delete;
  PipelineCache &operator=(const PipelineCache &) = delete;

  PipelineCache(const Device &crDevice, u64 initialDataSize,
                const void *pcInititalData, VkPipelineCacheCreateFlags flags);

  PipelineCache(PipelineCache &&rhs) noexcept;

  PipelineCache &operator=(PipelineCache &&rhs) noexcept;

  ~PipelineCache() noexcept { release(); }

  const VkPipelineCache &operator*() const noexcept { return mPCache; }

  constexpr bool empty() const noexcept { return mPCache == VK_NULL_HANDLE; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  VkPipelineCache mPCache = VK_NULL_HANDLE;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_CACHE_HPP */