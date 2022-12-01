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

class DescriptorPool : public vkw::DescriptorPool {
public:
  using vkw::DescriptorPool::DescriptorPool;

  DescriptorPool(const Device &device, uint32_t descriptorCount) {
    _initialize(device.ref(), descriptorCount);
  }

private:
  void _initialize(VkDevice device, uint32_t descriptorCount);
};

// class DescriptorSets : public vkw::DescriptorSets {
// public:
//   using vkw::DescriptorSets::DescriptorSets;

//   DescriptorSets(const Device &device, const DescriptorSetLayout &layout,
//                  DescriptorPool &pool, const std::vector<VkBuffer> &buffers,
//                  ImageView imageView, Sampler sampler) {
//     _initialize(device.ref(), layout.ref(), pool, buffers, imageView.ref(),
//                 sampler.ref());
//   }

// private:
//   void _initialize(VkDevice device, VkDescriptorSetLayout layout,
//                    DescriptorPool &pool, const std::vector<VkBuffer>
//                    &buffers, VkImageView imageView, VkSampler sampler);
// };

} /* namespace rtvc */
#endif /* RTVC_DESCRIPTOR_HPP */