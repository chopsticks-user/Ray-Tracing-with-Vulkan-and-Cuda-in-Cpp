#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "settings.hpp"

#include "renderer/renderer.hpp"

namespace neko {

class Engine : public StaticObject {
public:
  using StaticObject::StaticObject;

  virtual ~Engine() = default;

  void start() { mRenderer.start(); };

protected:
  Settings mSettings = {};
  Renderer mRenderer = Renderer{mSettings};
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */