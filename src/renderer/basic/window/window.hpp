#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "utils.hpp"

namespace neko {

class Window {
  typedef GLFWwindow *GLWindow;

public:
  Window() = default;

  explicit Window(const Settings &settings);

  Window(const Window &) = delete;

  Window(Window &&) = default;

  Window &operator=(const Window &) = delete;

  Window &operator=(Window &&) = default;

  ~Window();

  const GLWindow &operator*() const noexcept { return mWindow; }

  void open();

private:
  GLWindow mWindow;
  u32 mWidth;
  u32 mHeight;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_WINDOW_HPP */