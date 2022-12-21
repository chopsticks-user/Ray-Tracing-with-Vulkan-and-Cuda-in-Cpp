#include "sync.hpp"

#include "logical_device.hpp"

namespace neko {

RenderSync::RenderSync(const Device &crDevice, u64 frameCount)
    : mpcDevice{&crDevice}, mImageAvailableSemaphores{frameCount},
      mRenderFinishedSemaphores{frameCount}, mInFlightFences{frameCount} {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphoreInfo.pNext = nullptr;
  semaphoreInfo.flags = 0;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.pNext = nullptr;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (u64 iSync = 0; iSync < frameCount; ++iSync) {
    vkCreateSemaphore(*crDevice, &semaphoreInfo, nullptr,
                      &mImageAvailableSemaphores[iSync]);
    vkCreateSemaphore(*crDevice, &semaphoreInfo, nullptr,
                      &mRenderFinishedSemaphores[iSync]);
    vkCreateFence(*crDevice, &fenceInfo, nullptr, &mInFlightFences[iSync]);
  }
}

RenderSync::RenderSync(RenderSync &&rhs) noexcept
    : mImageAvailableSemaphores{std::move(rhs.mImageAvailableSemaphores)},
      mRenderFinishedSemaphores{std::move(rhs.mRenderFinishedSemaphores)},
      mInFlightFences{std::move(rhs.mInFlightFences)} {
  rhs.clearContainers();
}

RenderSync &RenderSync::operator=(RenderSync &&rhs) noexcept {
  this->release();
  mImageAvailableSemaphores = std::move(rhs.mImageAvailableSemaphores);
  mRenderFinishedSemaphores = std::move(rhs.mRenderFinishedSemaphores);
  mInFlightFences = std::move(rhs.mInFlightFences);
  rhs.clearContainers();
  return *this;
}

void RenderSync::release() noexcept {
  if (!mImageAvailableSemaphores.empty()) {
    for (u64 iSync = 0; iSync < mImageAvailableSemaphores.size(); ++iSync) {
      vkDestroySemaphore(**mpcDevice, mImageAvailableSemaphores[iSync],
                         nullptr);
      vkDestroySemaphore(**mpcDevice, mRenderFinishedSemaphores[iSync],
                         nullptr);
      vkDestroyFence(**mpcDevice, mInFlightFences[iSync], nullptr);
    }
    clearContainers();
  }
}

void RenderSync::clearContainers() noexcept {
  mImageAvailableSemaphores.clear();
  mRenderFinishedSemaphores.clear();
  mInFlightFences.clear();
}

} /* namespace neko */