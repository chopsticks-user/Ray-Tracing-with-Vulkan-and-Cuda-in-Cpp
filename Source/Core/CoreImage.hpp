#ifndef NEKO_CORE_IMAGE_HPP
#define NEKO_CORE_IMAGE_HPP

#include "CoreDefines.hpp"

#include <cmath>

namespace Neko
{
    namespace detail
    {

        enum class ImageColor
        {
            none = 0,
            g = 1,
            ga = 2,
            rgb = 3,
            rgba = 4,
        };

        class LoadedImage
        {
        public:
            LoadedImage() = default;
            LoadedImage(const LoadedImage &) = delete;
            LoadedImage &operator=(const LoadedImage &) = delete;

            LoadedImage(const std::string &imageFilePath,
                        ImageColor color = ImageColor::rgba);

            LoadedImage(LoadedImage &&rhs) noexcept;

            LoadedImage &operator=(LoadedImage &&rhs) noexcept;

            ~LoadedImage() noexcept { release(); };

            const void *pixels() const noexcept { return mpPixels; }

            i32 width() const noexcept { return mWidth; }

            i32 height() const noexcept { return mHeight; }

            i32 channels() const noexcept { return mChannels; }

            u32 mipLevels() const noexcept
            {
                if (mWidth > 0 && mHeight > 0)
                {
                    return vku32(std::floor(std::log2(std::max(mWidth, mHeight)))) + 1;
                }
                return 1;
            }

            ImageColor color() const noexcept { return mColor; }

            u64 imageSize() const noexcept
            {
                return static_cast<u64>(mWidth * mHeight * 4);
            }

            void release() noexcept;

        private:
            void *mpPixels = nullptr;
            i32 mWidth = 0;
            i32 mHeight = 0;
            i32 mChannels = 0;
            ImageColor mColor = ImageColor::none;
        };

    } /* namespace detail */

} /* namespace Neko */

#endif /* NEKO_CORE_IMAGE_HPP */