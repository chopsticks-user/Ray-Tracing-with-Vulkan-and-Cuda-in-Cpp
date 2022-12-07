#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "basic/context/context.hpp"
#include "basic/instance/instance.hpp"
#include "basic/surface/surface.hpp"
#include "basic/window/window.hpp"
#include "devices/logical/logical.hpp"
#include "devices/physical/physical.hpp"
// #include "devices/queues/queues.hpp"

#include <memory>

namespace neko {

class Renderer {
public:
  Renderer() = delete;
  explicit Renderer(const Settings &settings) : mSettings{settings} {}
  Renderer(const Renderer &) = delete;
  Renderer(Renderer &&) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = delete;
  ~Renderer() = default;
  void start() { mWindow.open(); };

private:
  const Settings &mSettings;

  Context mContext;

  Instance mInstance{
      mSettings,
      mContext,
  };

  Window mWindow{
      mSettings,
  };

  Surface mSurface{
      mInstance,
      mWindow,
  };
};

} /* namespace neko */

#endif /* NEKO_RENDERER_HPP */