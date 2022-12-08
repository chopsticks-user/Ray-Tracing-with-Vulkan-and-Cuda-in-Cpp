#ifndef NEKO_RENDERER_BASIC_WINDOW_HPP
#define NEKO_RENDERER_BASIC_WINDOW_HPP

#include "settings.hpp"

namespace neko {

class Window : public StaticObject {
  typedef GLFWwindow *GLWindow;

public:
  using StaticObject::StaticObject;

  explicit Window(const Settings &settings);

  virtual ~Window();

  const GLWindow &operator*() const noexcept { return mWindow; }

  void open();

protected:
  GLWindow mWindow;
  uint32_t mWidth;
  uint32_t mHeight;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_BASIC_WINDOW_HPP */