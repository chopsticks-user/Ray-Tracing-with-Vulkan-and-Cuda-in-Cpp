#ifndef VKW_DESCRIPTOR_SETS_HPP
#define VKW_DESCRIPTOR_SETS_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class DescriptorSets {
public:
  DescriptorSets() = default;
  DescriptorSets(VkDevice device, VkDescriptorPool pool,
                 const VkDescriptorSetAllocateInfo *pAllocateInfo)
      : _sets{pAllocateInfo->descriptorSetCount}, _device{device}, _pool{pool} {
    if (vkAllocateDescriptorSets(device, pAllocateInfo, _sets.data()) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate descriptor sets.");
    }
    _isOwner = true;
  }
  DescriptorSets(const DescriptorSets &) = delete;
  DescriptorSets(DescriptorSets &&rhs) { _moveDataFrom(std::move(rhs)); }
  DescriptorSets &operator=(const DescriptorSets &) = delete;
  DescriptorSets &operator=(DescriptorSets &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~DescriptorSets() { _destroyVkData(); }

  const std::vector<VkDescriptorSet> &ref() const noexcept { return _sets; }

  const VkDescriptorSet &operator[](size_t index) const { return _sets[index]; }

private:
  std::vector<VkDescriptorSet> _sets;
  VkDevice _device;
  VkDescriptorPool _pool;
  bool _isOwner = false;

  void _moveDataFrom(DescriptorSets &&rhs) {
    _sets = std::move(rhs._sets);
    _device = rhs._device;
    _pool = rhs._pool;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkFreeDescriptorSets(_device, _pool, static_cast<uint32_t>(_sets.size()),
                           _sets.data());
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "DescriptorSets destructor" << '\n';
      }
    }
  }
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_SETS_HPP */