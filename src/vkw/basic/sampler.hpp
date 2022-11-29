#ifndef VKW_BASIC_SAMPLER_HPP
#define VKW_BASIC_SAMPLER_HPP

namespace vkw {
class Sampler {
public:
  Sampler() = default;
  Sampler(VkDevice device, VkPhysicalDevice physicalDevice) : _device{device} {
    _customInitialize(device, physicalDevice);
  }
  Sampler(VkDevice device, const VkSamplerCreateInfo *pCreateInfo,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    if (vkCreateSampler(device, pCreateInfo, pAllocator, &_sampler) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create sampler.");
    }
    _isOwner = true;
  }
  Sampler(const Sampler &) = delete;
  Sampler(Sampler &&rhs) { _moveDataFrom(std::move(rhs)); }
  Sampler &operator=(const Sampler &) = delete;
  Sampler &operator=(Sampler &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Sampler() { _destroyVkData(); }

  const VkSampler &ref() const noexcept { return _sampler; }

protected:
  VkSampler _sampler = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Sampler &&rhs) {
    _destroyVkData();
    _sampler = rhs._sampler;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroySampler(_device, _sampler, _pAllocator);
      _isOwner = false;
      if (enableValidationLayers) {
        std::cout << "Sampler destructor" << '\n';
      }
    }
  }

private:
  void _customInitialize(VkDevice device, VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    if (vkCreateSampler(device, &samplerInfo, nullptr, &_sampler) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create sampler.");
    }
    _device = device;
    _isOwner = true;
  }
};
} // namespace vkw
#endif /* VKW_BASIC_SAMPLER_HPP */