#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "settings.hpp"

#include "renderer/renderer.hpp"

#include <memory>

namespace neko {

class Engine {
public:
  Engine() = default;

  Engine(const Engine &) = delete;

  Engine(Engine &&) = default;

  Engine &operator=(const Engine &) = delete;

  Engine &operator=(Engine &&) = default;

  ~Engine() = default;

  void start() { mRenderer->start(); };

private:
  std::unique_ptr<Settings> mSettings = std::make_unique<Settings>();
  std::unique_ptr<Renderer> mRenderer = std::make_unique<Renderer>(*mSettings);
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */