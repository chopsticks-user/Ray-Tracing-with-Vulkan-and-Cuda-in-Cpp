#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "settings.hpp"

#include "renderer/renderer.hpp"

namespace neko {

class Engine {
public:
  Engine() = default;
  ~Engine() = default;
  void start() { mRenderer.start(); };

private:
  Settings mSettings;
  Renderer mRenderer{mSettings};
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */