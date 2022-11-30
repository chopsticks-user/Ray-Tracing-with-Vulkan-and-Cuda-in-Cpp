#ifndef RTVC_DESCRIPTOR_HPP
#define RTVC_DESCRIPTOR_HPP

#include "device.hpp"

namespace rtvc {

class DescriptorSetLayout : public vkw::DescriptorSetLayout {
public:
  using vkw::DescriptorSetLayout::DescriptorSetLayout;

  DescriptorSetLayout(const Device &device) { _initialize(device.ref()); }

private:
  void _initialize(VkDevice device);
};

} /* namespace rtvc */
#endif /* RTVC_DESCRIPTOR_HPP */