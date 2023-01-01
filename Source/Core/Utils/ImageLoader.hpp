#ifndef HDG_CORE_IMAGELOADER_HPP
#define HDG_CORE_IMAGELOADER_HPP

#include "Common/Common.hpp"

#include <cmath>

namespace Neko::Core
{
    namespace Enum
    {
        enum class ImageColor
        {
            none = 0,
            g = 1,
            ga = 2,
            rgb = 3,
            rgba = 4,
        };

    } // namespace Neko::Core::Enum

    class ImageLoader
    {
    public:
        ImageLoader() = default;
        ImageLoader(const ImageLoader &) = delete;
        ImageLoader &operator=(const ImageLoader &) = delete;

        ImageLoader(const std::string &imageFilePath,
                    Enum::ImageColor color = Enum::ImageColor::rgba);

        ImageLoader(ImageLoader &&rhs) noexcept;

        ImageLoader &operator=(ImageLoader &&rhs) noexcept;

        ~ImageLoader() noexcept { release(); };

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

        Enum::ImageColor color() const noexcept { return mColor; }

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
        Enum::ImageColor mColor = Enum::ImageColor::none;
    };

} // namespace Neko::Core

#endif /* HDG_CORE_IMAGELOADER_HPP */