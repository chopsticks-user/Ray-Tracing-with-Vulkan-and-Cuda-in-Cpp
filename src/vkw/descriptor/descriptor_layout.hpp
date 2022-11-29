#ifndef VKW_DESCRIPTOR_LAYOUT_HPP
#define VKW_DESCRIPTOR_LAYOUT_HPP

#include "config.hpp"

#include <array>
#include <vkh.hpp>

namespace vkw {

class DescriptorSetLayout {
public:
  DescriptorSetLayout() = default;
  DescriptorSetLayout(VkDevice device) : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device);
  }
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
  ~DescriptorSetLayout() { _destroyVkData(); }

  const VkDescriptorSetLayout &ref() const noexcept { return _setLayout; }

private:
  VkDescriptorSetLayout _setLayout;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
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
      if constexpr (enableValidationLayers) {
        std::cout << "DescriptorSetLayout destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device) {
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

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
                                    &_setLayout) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor set layout.");
    }
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_LAYOUT_HPP */