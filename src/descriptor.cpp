#include "descriptor.hpp"

namespace rtvc {

void DescriptorSetLayout::_initialize(VkDevice device) {
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

  if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &_setLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout.");
  }
  _device = device;
  _isOwner = true;
}

void DescriptorPool::_initialize(VkDevice device, uint32_t descriptorCount) {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = static_cast<uint32_t>(descriptorCount);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = static_cast<uint32_t>(descriptorCount);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = static_cast<uint32_t>(descriptorCount);
  poolInfo.flags = DescriptorPool::requiredFlag;
  if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &_pool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool.");
  }
  _device = device;
  _isOwner = true;
}

// void DescriptorSets::_initialize(VkDevice device, VkDescriptorSetLayout
// layout,
//                                  DescriptorPool &pool,
//                                  const std::vector<VkBuffer> &buffers,
//                                  VkImageView imageView, VkSampler sampler) {
//   std::vector<VkDescriptorSetLayout> layouts{buffers.size(), layout};
//   _sets = pool.allocateSets(layouts);
//   for (size_t i = 0; i < buffers.size(); ++i) {
//     VkDescriptorBufferInfo bufferInfo{};
//     bufferInfo.buffer = buffers[i];
//     bufferInfo.offset = 0;
//     bufferInfo.range = sizeof(vkw::UniformBufferObject);

//     VkDescriptorImageInfo imageInfo{};
//     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//     imageInfo.imageView = imageView;
//     imageInfo.sampler = sampler;

//     std::array<VkWriteDescriptorSet, 2> writeSets{};

//     writeSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     writeSets[0].dstSet = sets[i];
//     writeSets[0].dstBinding = 0;
//     writeSets[0].dstArrayElement = 0;
//     writeSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     writeSets[0].descriptorCount = 1;
//     writeSets[0].pBufferInfo = &bufferInfo;
//     writeSets[0].pImageInfo = nullptr;
//     writeSets[0].pTexelBufferView = nullptr;

//     writeSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     writeSets[1].dstSet = sets[i];
//     writeSets[1].dstBinding = 1;
//     writeSets[1].dstArrayElement = 0;
//     writeSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     writeSets[1].descriptorCount = 1;
//     writeSets[1].pBufferInfo = nullptr;
//     writeSets[1].pImageInfo = &imageInfo;
//     writeSets[1].pTexelBufferView = nullptr;

//     vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeSets.size()),
//                            writeSets.data(), 0, nullptr);
//   }

} /* namespace rtvc */