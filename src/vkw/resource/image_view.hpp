#ifndef VKW_IMAGEVIEW_HPP
#define VKW_IMAGEVIEW_HPP

#include "config.hpp"

namespace vkw {

class ImageView {
public:
  ImageView() = default;
  ImageView(VkDevice device, const VkImageViewCreateInfo *pCreateInfo,
            const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    vkCreateImageView(device, pCreateInfo, nullptr, &_imageView);
    _isOwner = true;
  }
  ImageView(const ImageView &) = delete;
  ImageView(ImageView &&rhs) { _moveDataFrom(std::move(rhs)); }
  ImageView &operator=(const ImageView &) = delete;
  ImageView &operator=(ImageView &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }

  virtual ~ImageView() { _destroyVkData(); }

  const VkImageView &ref() { return _imageView; }

protected:
  VkImageView _imageView = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(ImageView &&rhs) {
    _destroyVkData();
    _imageView = rhs._imageView;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyImageView(_device, _imageView, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "ImageView destructor" << '\n';
      }
    }
  }

private:
};

class ImageViews {
public:
  ImageViews() = default;
  ImageViews(VkDevice device,
             const std::vector<VkImageViewCreateInfo> &createInfos,
             const VkAllocationCallbacks *pAllocator = nullptr)
      : _imageViews{createInfos.size()}, _device{device}, _pAllocator{
                                                              pAllocator} {
    size_t imageCount = createInfos.size();
    for (size_t i = 0; i < imageCount; ++i) {
      vkCreateImageView(device, &createInfos[i], nullptr, &_imageViews[i]);
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

  virtual ~ImageViews() { _destroyVkData(); }

  const std::vector<VkImageView> &ref() const noexcept { return _imageViews; }

  template <typename SizeType = size_t> auto size() const noexcept {
    return static_cast<SizeType>(_imageViews.size());
  }

  const VkImageView &operator[](size_t index) const noexcept {
    return _imageViews[index];
  }

protected:
  std::vector<VkImageView> _imageViews = {};
  VkDevice _device = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
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
        vkDestroyImageView(_device, imageView, _pAllocator);
      }
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Imageviews destructor" << '\n';
      }
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_IMAGEVIEW_HPP */