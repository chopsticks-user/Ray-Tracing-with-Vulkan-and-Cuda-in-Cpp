#ifndef VKW_DESCRIPTOR_POOL_HPP
#define VKW_DESCRIPTOR_POOL_HPP

#include "config.hpp"

#include "descriptor_sets.hpp"

#include <array>
#include <vector>

namespace vkw {

class DescriptorPool {
public:
  static constexpr VkDescriptorPoolCreateFlags requiredFlag =
      VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

  DescriptorPool() = default;
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
  virtual ~DescriptorPool() { _destroyVkData(); }

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

protected:
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
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_POOL_HPP */