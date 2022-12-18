#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "utils.hpp"

namespace neko {

class Window {
  typedef GLFWwindow *GLWindow;

public:
  Window() = default;
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  explicit Window(const Settings &settings);

  Window(Window &&) noexcept;

  Window &operator=(Window &&) noexcept;

  ~Window() noexcept { release(); };

  const GLWindow &operator*() const noexcept { return mWindow; }

  void release() noexcept;

  void open();

private:
  GLWindow mWindow = nullptr;
  u32 mWidth = 0;
  u32 mHeight = 0;
  bool mIsOwner = false;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_WINDOW_HPP */