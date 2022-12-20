#include "render_pass.hpp"

#include "logical_device.hpp"

namespace neko {

RenderPass::RenderPass(const Device &crDevice,
                       const VkRenderPassCreateInfo &crRenderPassInfo)
    : mpcDevice{&crDevice} {
  if (vkCreateRenderPass(*crDevice, &crRenderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass");
  }
}

RenderPass::RenderPass(RenderPass &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)},
      mRenderPass{std::exchange(rhs.mRenderPass, VK_NULL_HANDLE)} {}

RenderPass &RenderPass::operator=(RenderPass &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mRenderPass = std::exchange(rhs.mRenderPass, VK_NULL_HANDLE);
  return *this;
}

void RenderPass::release() noexcept {
  if (mRenderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(**mpcDevice, mRenderPass, nullptr);
    mRenderPass = VK_NULL_HANDLE;
  }
}

} /* namespace neko */