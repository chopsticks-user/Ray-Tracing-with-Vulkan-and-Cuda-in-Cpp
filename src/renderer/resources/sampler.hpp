#ifndef NEKO_RENDERER_RESOURCES_SAMPLER_HPP
#define NEKO_RENDERER_RESOURCES_SAMPLER_HPP

#include "core.hpp"

namespace neko {

class Device;

class Sampler {
public:
  Sampler() = default;
  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;

  Sampler(const Configs &crConfig, const Device &crDevice, u32 mipLevels = 1);

  Sampler(Sampler &&rhs) noexcept;

  Sampler &operator=(Sampler &&rhs) noexcept;

  ~Sampler() noexcept { release(); }

  const VkSampler &operator*() const noexcept { return mSampler; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  VkSampler mSampler = VK_NULL_HANDLE;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_SAMPLER_HPP */