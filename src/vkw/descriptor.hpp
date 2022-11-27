#ifndef VKW_DESCRIPTOR_HPP
#define VKW_DESCRIPTOR_HPP

#include "config.hpp"

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

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
                                    &_setLayout) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor set layout.");
    }
    _isOwner = true;
  }
};

class DescriptorPool {
public:
  DescriptorPool() = default;
  DescriptorPool(VkDevice device, uint32_t descriptorCount)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, descriptorCount);
  }
  DescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo *pCreateInfo,
                 const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    vkCreateDescriptorPool(device, pCreateInfo, pAllocator, &_pool);
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

  std::vector<VkDescriptorSet>
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

    std::vector<VkDescriptorSet> descriptorSets{setCount};
    descriptorSets.resize(setCount);
    if (vkAllocateDescriptorSets(_device, &allocInfo, descriptorSets.data()) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate descriptor sets.");
    }
    return descriptorSets;
  }

  void
  updateSets(const std::vector<VkWriteDescriptorSet> &descriptorWrites,
             const std::vector<VkCopyDescriptorSet> &descriptorCopies = {}) {
    /* If size() is ​0​, data() may or may not return a null
    pointer. */
    uint32_t copyCount = static_cast<uint32_t>(descriptorCopies.size());
    const VkCopyDescriptorSet *pDescriptorSet = nullptr;
    if (copyCount > 0) {
      pDescriptorSet = descriptorCopies.data();
    }
    vkUpdateDescriptorSets(_device,
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), copyCount, pDescriptorSet);
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
    VkDescriptorPoolSize descriptorPoolSize{};
    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = static_cast<uint32_t>(descriptorCount);

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = 1;
    descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
    descriptorPoolInfo.maxSets = static_cast<uint32_t>(descriptorCount);

    if (vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &_pool) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor pool.");
    }
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_DESCRIPTOR_HPP */