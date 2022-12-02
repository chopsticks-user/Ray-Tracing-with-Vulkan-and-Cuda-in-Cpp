#ifndef RTVC_SAMPLER_HPP
#define RTVC_SAMPLER_HPP

#include "device.hpp"

namespace rtvc {

class Sampler : public vkw::Sampler {
public:
  using vkw::Sampler::Sampler;

  Sampler(const Device &device, uint32_t mipLevels) {
    _initialize(device.ref(), device.physical(), mipLevels);
  }

private:
  void _initialize(VkDevice device, VkPhysicalDevice physicalDevice,
                   uint32_t mipLevels);
};

} /* namespace rtvc */
#endif /* RTVC_SAMPLER_HPP */