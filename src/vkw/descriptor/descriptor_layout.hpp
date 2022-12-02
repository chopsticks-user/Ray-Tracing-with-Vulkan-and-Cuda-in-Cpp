#ifndef VKW_DESCRIPTOR_LAYOUT_HPP
#define VKW_DESCRIPTOR_LAYOUT_HPP

#include "config.hpp"

#include <array>

namespace vkw {

class DescriptorSetLayout {
public:
  DescriptorSetLayout() = default;
  DescriptorSetLayout(VkDevice device,
                      const VkDescriptorSetLayoutCreateInfo *pCreateInfo,
                      const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, &_setLayout);
    _isOwner = true;
  }
  DescriptorSetLayout(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout(DescriptorSetLayout &&rhs) {
    _moveDataFrom(std::move(rhs));
  }
  DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout &operator=(DescriptorSetLayout &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~DescriptorSetLayout() { _destroyVkData(); }

  const VkDescriptorSetLayout &ref() const noexcept { return _setLayout; }

protected:
  VkDescriptorSetLayout _setLayout = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(DescriptorSetLayout &&rhs) {
    _setLayout = rhs._setLayout;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyDescriptorSetLayout(_device, _setLayout, _pAllocator);
      _isOwner = false;
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_LAYOUT_HPP */