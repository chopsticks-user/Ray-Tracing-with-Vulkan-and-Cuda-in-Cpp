#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "settings.hpp"

namespace neko {

class Window : public StaticObject {
  typedef GLFWwindow *GLWindow;

public:
  using StaticObject::StaticObject;

  explicit Window(const Settings &settings);

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