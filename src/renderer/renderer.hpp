#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "basic/context/context.hpp"
#include "basic/instance/instance.hpp"
#include "basic/surface/surface.hpp"
#include "basic/window/window.hpp"
#include "commands/commands.hpp"
#include "devices/logical/logical.hpp"

#include <memory>

namespace neko {

class Renderer : public StaticObject {
public:
  using StaticObject::StaticObject;

  explicit Renderer(const Settings &settings) : mSettings{settings} {}

  ~Renderer() = default;

  void start() { mWindow.open(); };

private:
  const Settings &mSettings;

  Context mContext{};

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

  LogicalDevice mDevice{
      mInstance,
      mSurface,
  };
};

} /* namespace neko */

#endif /* NEKO_RENDERER_HPP */