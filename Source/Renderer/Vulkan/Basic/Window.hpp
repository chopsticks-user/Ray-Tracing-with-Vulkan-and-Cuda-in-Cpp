#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "CoreNeko.hpp"

#include <functional>

namespace Neko
{

    class Window
    {
        typedef GLFWwindow *GLWindow;

    public:
        static constexpr bool resizeable = true;

        Window() = default;
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        explicit Window(const EngineConfigs &settings, bool windowResizeable = false);

        Window(Window &&) noexcept;

        Window &operator=(Window &&) noexcept;

        ~Window() noexcept { release(); };

        const GLWindow &operator*() const noexcept { return mWindow; }

        operator GLFWwindow *() const noexcept { return mWindow; }

        void whileMinimize() noexcept;

        void release() noexcept;

    private:
        GLWindow mWindow = nullptr;
        u32 mWidth = 0;
        u32 mHeight = 0;
    };

} // namespace Neko

#endif /* NEKO_RENDERER_BASIC_WINDOW_HPP */