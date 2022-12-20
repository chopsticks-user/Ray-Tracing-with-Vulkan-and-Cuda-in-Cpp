#include "descriptor.hpp"

#include "logical_device.hpp"

#include <array>

namespace neko {

DescriptorSetLayout::DescriptorSetLayout(
    [[maybe_unused]] const Configs &crConfig, const Device &crDevice)
    : mpcDevice{&crDevice} {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  samplerLayoutBinding.pImmutableSamplers = nullptr;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding,
                                                          samplerLayoutBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(**mpcDevice, &layoutInfo, nullptr,
                                  &mDSLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout");
  }
}

DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mDSLayout{std::exchange(
                                               rhs.mDSLayout, VK_NULL_HANDLE)} {
}

DescriptorSetLayout &
DescriptorSetLayout::operator=(DescriptorSetLayout &&rhs) noexcept {
  this->release();
  mpcDevice = std::move(rhs.mpcDevice);
  mDSLayout = std::exchange(rhs.mDSLayout, VK_NULL_HANDLE);
  return *this;
}

void DescriptorSetLayout::release() noexcept {
  if (mDSLayout != VK_NULL_HANDLE) {
    vkDestroyDescriptorSetLayout(**mpcDevice, mDSLayout, nullptr);
    mDSLayout = VK_NULL_HANDLE;
  }
}

DescriptorPool::DescriptorPool([[maybe_unused]] const Configs &crConfig,
                               const Device &crDevice, u32 descriptorSetCount)
    : mpcDevice{&crDevice} {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = static_cast<uint32_t>(descriptorSetCount);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = static_cast<uint32_t>(descriptorSetCount);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = static_cast<uint32_t>(descriptorSetCount);
  poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  if (vkCreateDescriptorPool(**mpcDevice, &poolInfo, nullptr, &mDPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool");
  }
}

DescriptorPool::DescriptorPool(DescriptorPool &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mDPool{std::exchange(
                                               rhs.mDPool, VK_NULL_HANDLE)} {}

DescriptorPool &DescriptorPool::operator=(DescriptorPool &&rhs) noexcept {
  this->release();
  mpcDevice = std::move(rhs.mpcDevice);
  mDPool = std::exchange(rhs.mDPool, VK_NULL_HANDLE);
  return *this;
}

std::vector<VkDescriptorSet> DescriptorPool::alloc(
    const std::vector<VkDescriptorSetLayout> &dsLayouts) const {
  uint32_t dSetCount = static_cast<uint32_t>(dsLayouts.size());
  const VkDescriptorSetLayout *pSetLayouts = nullptr;
  if (dSetCount > 0) {
    pSetLayouts = dsLayouts.data();
  }

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.pNext = nullptr;
  allocInfo.descriptorPool = mDPool;
  allocInfo.descriptorSetCount = dSetCount;
  allocInfo.pSetLayouts = pSetLayouts;

  std::vector<VkDescriptorSet> descriptorSets{stdu64(dSetCount)};
  if (vkAllocateDescriptorSets(**mpcDevice, &allocInfo,
                               descriptorSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets");
  }

  return descriptorSets;
}

void DescriptorPool::free(const std::vector<VkDescriptorSet> dSets) const {
  if (vkFreeDescriptorSets(**mpcDevice, mDPool, vku32(dSets.size()),
                           dSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to free descriptor sets");
  }
}

void DescriptorPool::free(u32 dSetCount, const VkDescriptorSet *pDSets) const {
  if (vkFreeDescriptorSets(**mpcDevice, mDPool, dSetCount, pDSets) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to free descriptor sets");
  }
}

void DescriptorPool::release() noexcept {
  if (mDPool != VK_NULL_HANDLE) {
    vkDestroyDescriptorPool(**mpcDevice, mDPool, nullptr);
    mDPool = VK_NULL_HANDLE;
  }
}

} /* namespace neko */