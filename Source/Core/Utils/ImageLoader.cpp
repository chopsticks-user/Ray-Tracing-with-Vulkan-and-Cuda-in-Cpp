#include "ImageLoader.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif /* STB_IMAGE_IMPLEMENTATION */
#include "stb_image.h"

namespace Neko::Core
{
    ImageLoader::ImageLoader(const std::string &imageFilePath,
                             Enum::ImageColor colorType)
    {
        mpPixels = stbi_load(imageFilePath.c_str(), &mWidth, &mHeight, &mChannels,
                             static_cast<i32>(colorType));
        if (!mpPixels)
        {
            throw std::runtime_error("Failed to load image file " + imageFilePath);
        }
    }

    ImageLoader::ImageLoader(ImageLoader &&rhs) noexcept
        : mpPixels{std::exchange(rhs.mpPixels, nullptr)}, mWidth{std::exchange(
                                                              rhs.mWidth, 0)},
          mHeight{std::exchange(rhs.mHeight, 0)}, mChannels{std::exchange(
                                                      rhs.mChannels, 0)},
          mColor{std::exchange(rhs.mColor, Enum::ImageColor::none)} {}

    ImageLoader &ImageLoader::operator=(ImageLoader &&rhs) noexcept
    {
        release();
        mpPixels = std::exchange(rhs.mpPixels, nullptr);
        mWidth = std::exchange(rhs.mWidth, 0);
        mHeight = std::exchange(rhs.mHeight, 0);
        mChannels = std::exchange(rhs.mChannels, 0);
        mColor = std::exchange(rhs.mColor, Enum::ImageColor::none);
        return *this;
    }

    void ImageLoader::release() noexcept
    {
        if (mpPixels != nullptr)
        {
            stbi_image_free(mpPixels);
            mpPixels = nullptr;
        }
    }

} /* namespace Neko::Core */
