#ifndef VKW_DESCRIPTOR_POOL_HPP
#define VKW_DESCRIPTOR_POOL_HPP

#include "config.hpp"

#include "descriptor_sets.hpp"

#include <array>
#include <vkh.hpp>

namespace vkw {

class DescriptorPool {
public:
  static constexpr VkDescriptorPoolCreateFlags requiredFlag =
      VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

  DescriptorPool() = default;
  DescriptorPool(VkDevice device, uint32_t descriptorCount)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, descriptorCount);
  }
  DescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo *pCreateInfo,
                 const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    if (pCreateInfo->flags == 0 ||
        (pCreateInfo->flags & requiredFlag) != pCreateInfo->flags) {
      throw std::runtime_error(
          "The flag VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT is "
          "required.");
    }
    if (vkCreateDescriptorPool(device, pCreateInfo, pAllocator, &_pool) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor pool.");
    }
    _isOwner = true;
  }
  DescriptorPool(const DescriptorPool &) = delete;
  DescriptorPool(DescriptorPool &&rhs) { _moveDataFrom(std::move(rhs)); }
  DescriptorPool &operator=(const DescriptorPool &) = delete;
  DescriptorPool &operator=(DescriptorPool &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~DescriptorPool() { _destroyVkData(); }

  const VkDescriptorPool &ref() const noexcept { return _pool; }

  vkw::DescriptorSets
  allocateSets(const std::vector<VkDescriptorSetLayout> &setLayouts,
               const void *pNext = nullptr) {
    uint32_t setCount = static_cast<uint32_t>(setLayouts.size());
    const VkDescriptorSetLayout *pSetLayouts = nullptr;
    if (setCount > 0) {
      pSetLayouts = setLayouts.data();
    }
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = pNext;
    allocInfo.descriptorPool = _pool;
    allocInfo.descriptorSetCount = setCount;
    allocInfo.pSetLayouts = pSetLayouts;
    return {_device, _pool, &allocInfo};
  }

private:
  VkDescriptorPool _pool;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(DescriptorPool &&rhs) {
    _pool = rhs._pool;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyDescriptorPool(_device, _pool, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "DescriptorPool destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device, uint32_t descriptorCount) {
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
    poolInfo.flags = requiredFlag;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &_pool) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor pool.");
    }
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_POOL_HPP */