#ifndef VKW_BASIC_SAMPLER_HPP
#define VKW_BASIC_SAMPLER_HPP

#include "config.hpp"

namespace vkw {
class Sampler {
public:
  Sampler() = default;
  Sampler(VkDevice device, const VkSamplerCreateInfo *pCreateInfo,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    if (vkCreateSampler(device, pCreateInfo, pAllocator, &_sampler) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create sampler.");
    }
    _isOwner = true;
  }
  Sampler(const Sampler &) = delete;
  Sampler(Sampler &&rhs) { _moveDataFrom(std::move(rhs)); }
  Sampler &operator=(const Sampler &) = delete;
  Sampler &operator=(Sampler &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Sampler() { _destroyVkData(); }

  const VkSampler &ref() const noexcept { return _sampler; }

protected:
  VkSampler _sampler = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Sampler &&rhs) {
    _destroyVkData();
    _sampler = rhs._sampler;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroySampler(_device, _sampler, _pAllocator);
      _isOwner = false;
    }
  }

private:
};
} // namespace vkw
#endif /* VKW_BASIC_SAMPLER_HPP */