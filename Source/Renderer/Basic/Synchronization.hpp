#ifndef NEKO_RENDERER_BASIC_SYNC_HPP
#define NEKO_RENDERER_BASIC_SYNC_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;

/**
 * @brief
 * ! All container members must have the same size
 */
class RenderSync {
public:
  RenderSync() = default;
  RenderSync(const RenderSync &) = delete;
  RenderSync &operator=(const RenderSync &) = delete;

  RenderSync(const Device &crDevice, u64 frameCount = 1);

  RenderSync(RenderSync &&rhs) noexcept;

  RenderSync &operator=(RenderSync &&rhs) noexcept;

  ~RenderSync() noexcept { release(); }

  const VkSemaphore &imageAvailableSemaphore(u64 index) const noexcept {
    return mImageAvailableSemaphores.at(index);
  }

  const VkSemaphore &renderFinishedSemaphore(u64 index) const noexcept {
    return mRenderFinishedSemaphores.at(index);
  }

  const VkFence &inFlightFence(u64 index) const {
    return mInFlightFences.at(index);
  }

  u64 size() const noexcept { return mImageAvailableSemaphores.size(); }

  u64 currentFrame() const noexcept { return mCurrentFrame; }

  u64 next() noexcept {
    ++mCurrentFrame;
    return mCurrentFrame %= mImageAvailableSemaphores.size();
  }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  std::vector<VkSemaphore> mImageAvailableSemaphores = {};
  std::vector<VkSemaphore> mRenderFinishedSemaphores = {};
  std::vector<VkFence> mInFlightFences = {};
  u64 mCurrentFrame = 0;

  void clearContainers() noexcept;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_SYNC_HPP */