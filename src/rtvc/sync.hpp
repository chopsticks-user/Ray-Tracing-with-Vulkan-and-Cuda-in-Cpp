#ifndef RTVC_SYNC_HPP
#define RTVC_SYNC_HPP

#include "device.hpp"

namespace rtvc {

struct SyncWrapper {
  std::vector<vkw::Semaphore> imageAvailableSemaphore;
  std::vector<vkw::Semaphore> renderFinishedSemaphore;
  std::vector<vkw::Fence> inFlightFence;
  size_t currentFrame = 0;

  SyncWrapper(const Device &device, size_t frameCount)
      : imageAvailableSemaphore{frameCount},
        renderFinishedSemaphore{frameCount}, inFlightFence{frameCount} {
    for (size_t i = 0; i < frameCount; ++i) {
      imageAvailableSemaphore[i] = {device.ref()};
      renderFinishedSemaphore[i] = {device.ref()};
      inFlightFence[i] = {device.ref(), VK_FENCE_CREATE_SIGNALED_BIT};
    }
  }
};

typedef std::vector<VkCommandBuffer> CommandBuffers;

} /* namespace rtvc */
#endif /* RTVC_SYNC_HPP */