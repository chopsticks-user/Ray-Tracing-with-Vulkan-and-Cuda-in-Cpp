#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "basic/context/context.hpp"
#include "basic/instance/instance.hpp"
#include "basic/surface/surface.hpp"
#include "basic/window/window.hpp"
#include "commands/commands.hpp"
#include "devices/logical/logical.hpp"

namespace neko {

class Renderer {
public:
  Renderer() = delete;

  explicit Renderer(const Settings &settings) : mpSettings{&settings} {}

  Renderer(const Renderer &) = delete;

  Renderer(Renderer &&) = default;

  Renderer &operator=(const Renderer &) = delete;

  Renderer &operator=(Renderer &&) = default;

  ~Renderer() = default;

  void start() {
    mWindow.open();
    // Instance instance = std::move(mInstance);
    // mInstance.release();
  };

private:
  const Settings *mpSettings = nullptr;
  Instance mInstance = Instance{*mpSettings};
  Window mWindow = Window{*mpSettings};
  Surface mSurface = {mInstance, mWindow};
  Device mDevice = {mInstance, mSurface};
};

} /* namespace neko */

#endif /* NEKO_RENDERER_HPP */