#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace neko {

namespace detail {

LoadedImage::LoadedImage(const std::string &imageFilePath,
                         ImageColor colorType) {
  mpPixels = stbi_load(imageFilePath.c_str(), &mWidth, &mHeight, &mChannels,
                       static_cast<i32>(colorType));
  if (!mpPixels) {
    throw std::runtime_error("Failed to load image file " + imageFilePath);
  }
}

LoadedImage::LoadedImage(LoadedImage &&rhs) noexcept
    : mpPixels{std::exchange(rhs.mpPixels, nullptr)}, mWidth{std::exchange(
                                                          rhs.mWidth, 0)},
      mHeight{std::exchange(rhs.mHeight, 0)}, mChannels{std::exchange(
                                                  rhs.mChannels, 0)},
      mColor{std::exchange(rhs.mColor, ImageColor::none)} {}

LoadedImage &LoadedImage::operator=(LoadedImage &&rhs) noexcept {
  release();
  mpPixels = std::exchange(rhs.mpPixels, nullptr);
  mWidth = std::exchange(rhs.mWidth, 0);
  mHeight = std::exchange(rhs.mHeight, 0);
  mChannels = std::exchange(rhs.mChannels, 0);
  mColor = std::exchange(rhs.mColor, ImageColor::none);
  return *this;
}

void LoadedImage::release() noexcept {
  if (mpPixels != nullptr) {
    stbi_image_free(mpPixels);
    mpPixels = nullptr;
  }
}

} /* namespace detail */

} /* namespace neko */