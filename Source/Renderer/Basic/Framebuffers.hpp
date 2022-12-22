#ifndef NEKO_RENDERER_BASIC_FRAMEBUFFERS_HPP
#define NEKO_RENDERER_BASIC_FRAMEBUFFERS_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;
class Swapchain;
class DepthBuffer;
class RenderPass;

class Framebuffers {
public:
  Framebuffers() = default;
  Framebuffers(const Framebuffers &) = delete;
  Framebuffers &operator=(const Framebuffers &) = delete;

  Framebuffers(const Device &crDevice, const Swapchain &crSwapchain,
               const RenderPass &crRenderPass,
               const DepthBuffer &crDepthBuffer);

  Framebuffers(Framebuffers &&rhs) noexcept;

  Framebuffers &operator=(Framebuffers &&rhs) noexcept;

  ~Framebuffers() noexcept { release(); }

  const std::vector<VkFramebuffer> &operator*() const noexcept {
    return mFramebuffers;
  }

  const VkFramebuffer &operator[](u64 index) const {
    return mFramebuffers.at(index);
  }

  auto size() const noexcept { return mFramebuffers.size(); }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  std::vector<VkFramebuffer> mFramebuffers = {};
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_FRAMEBUFFERS_HPP */