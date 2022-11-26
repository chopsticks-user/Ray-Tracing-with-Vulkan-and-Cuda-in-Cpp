#ifndef VKW_IMAGEVIEW_HPP
#define VKW_IMAGEVIEW_HPP

#include "config.hpp"

#include <vkh.hpp>

namespace vkw {

class ImageViews {
public:
  ImageViews() = default;
  ImageViews(VkDevice device, VkSwapchainKHR swapchain, VkFormat format)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, swapchain, format);
  }
  ImageViews(VkDevice device,
             const std::vector<VkImageViewCreateInfo> &createInfos,
             const VkAllocationCallbacks *pAllocator = nullptr)
      : _imageViews{createInfos.size()}, _device{device}, _pAllocator{
                                                              pAllocator} {
    size_t imageCount = createInfos.size();
    for (size_t i = 0; i < imageCount; ++i) {
      _imageViews[i] = vkh::createImageView(device, &createInfos[i]);
    }
    _isOwner = true;
  }
  ImageViews(const ImageViews &) = delete;
  ImageViews(ImageViews &&rhs) { _moveDataFrom(std::move(rhs)); }
  ImageViews &operator=(const ImageViews &) = delete;
  ImageViews &operator=(ImageViews &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }

  ~ImageViews() { _destroyVkData(); }

  const std::vector<VkImageView> &ref() { return _imageViews; }

  template <typename SizeType = size_t> auto size() const noexcept {
    return static_cast<SizeType>(_imageViews.size());
  }

  const VkImageView &operator[](size_t index) const noexcept {
    return _imageViews[index];
  }

private:
  std::vector<VkImageView> _imageViews;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(ImageViews &&rhs) {
    _destroyVkData();
    _imageViews = std::move(rhs._imageViews);
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      for (auto &imageView : _imageViews) {
        vkh::destroyImageView(_device, imageView, _pAllocator);
      }
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Framebuffers destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device, VkSwapchainKHR swapchain,
                                VkFormat format) {
    auto images = vkh::getSwapchainImages(device, swapchain);
    size_t imageCount = images.size();
    _imageViews.resize(imageCount);
    for (size_t i = 0; i < imageCount; ++i) {
      VkImageViewCreateInfo imageViewInfo{};
      imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewInfo.pNext = nullptr;
      // imageViewInfo.flags =
      imageViewInfo.image = images[i];

      /* treat images as 2D textures */
      imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

      imageViewInfo.format = format;

      /* default mapping */
      imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      /* color aspect */
      imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

      /* In stereographic 3D applications, create a swapchain with multiple
      layers before creating multiple image views for each images representing
      the views for the left and right eyes by accessing different layers */
      imageViewInfo.subresourceRange.baseMipLevel = 0;
      imageViewInfo.subresourceRange.levelCount = 1;
      imageViewInfo.subresourceRange.baseArrayLayer = 0;
      imageViewInfo.subresourceRange.layerCount = 1;

      _imageViews[i] = vkh::createImageView(device, &imageViewInfo);
    }
    _isOwner = true;
  }
};

} /* namespace vkw */
#endif /* VKW_IMAGEVIEW_HPP */