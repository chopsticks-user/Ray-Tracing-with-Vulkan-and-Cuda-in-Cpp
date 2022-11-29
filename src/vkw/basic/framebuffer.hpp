#ifndef VKW_FRAMEBUFFER_HPP
#define VKW_FRAMEBUFFER_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class Framebuffers {
public:
  Framebuffers() = default;
  Framebuffers(VkDevice device, const std::vector<VkImageView> &imageViews,
               VkRenderPass renderPass, const VkExtent2D &extent)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, imageViews, renderPass, extent);
  }
  Framebuffers(VkDevice device,
               const std::vector<VkFramebufferCreateInfo> &createInfos,
               const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    size_t framebufferCount = createInfos.size();
    _framebuffers.resize(createInfos.size());
    for (size_t i = 0; i < framebufferCount; ++i) {
      _framebuffers[i] =
          vkh::createFramebuffer(device, &createInfos[i], pAllocator);
    }
    _isOwner = true;
  }
  Framebuffers(const Framebuffers &) = delete;
  Framebuffers(Framebuffers &&rhs) { _moveDataFrom(std::move(rhs)); }
  Framebuffers &operator=(const Framebuffers &) = delete;
  Framebuffers &operator=(Framebuffers &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Framebuffers() { _destroyVkData(); }

  const std::vector<VkFramebuffer> &ref() const noexcept {
    return _framebuffers;
  }

  template <typename SizeType = size_t> auto size() const noexcept {
    return static_cast<SizeType>(_framebuffers.size());
  }

  const VkFramebuffer &operator[](size_t index) const noexcept {
    return _framebuffers[index];
  }

protected:
  std::vector<VkFramebuffer> _framebuffers;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Framebuffers &&rhs) {
    _destroyVkData();
    _framebuffers = std::move(rhs._framebuffers);
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      for (auto &framebuffer : _framebuffers) {
        vkh::destroyFramebuffer(_device, framebuffer, _pAllocator);
      }
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Framebuffers destructor" << '\n';
      }
    }
  }

private:
  void _customInitialize(VkDevice device,
                         const std::vector<VkImageView> &imageViews,
                         VkRenderPass renderPass, VkExtent2D extent) {
    size_t imageCount = imageViews.size();
    _framebuffers.resize(imageCount);
    std::vector<VkImageView> attachments{imageViews};
    for (size_t i = 0; i < imageViews.size(); ++i) {
      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = &attachments[i];
      framebufferInfo.width = extent.width;
      framebufferInfo.height = extent.height;
      framebufferInfo.layers = 1;
      _framebuffers[i] = vkh::createFramebuffer(device, &framebufferInfo);
    }
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_FRAMEBUFFER_HPP */