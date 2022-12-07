#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "settings.hpp"

namespace neko {

class Window {
  typedef GLFWwindow *GLWindow;

public:
  Window() = delete;
  explicit Window(const Settings &settings);
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;
  ~Window();

  const GLWindow &operator*() const noexcept { return mWindow; }

  void open();

private:
  GLWindow mWindow;
  uint32_t mWidth;
  uint32_t mHeight;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_WINDOW_HPP */