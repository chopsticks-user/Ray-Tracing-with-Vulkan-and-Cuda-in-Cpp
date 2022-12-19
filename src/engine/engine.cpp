#include "engine.hpp"

#include "renderer.hpp"
#include "threads.hpp"

namespace neko {

Engine::Engine(const std::string &settingsFilePath) {
  mpSettings = std::make_unique<Configs>(settingsFilePath);

  mpThreadPool = std::make_unique<ThreadPool>(*mpSettings);

  // mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);

  auto rendererReady = mpThreadPool->submitJob([&] {
    mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
  });
  rendererReady->wait();
};

Engine::~Engine() = default;

void Engine::start() {
  // mpRenderer->start();
  auto rendererStopped = mpThreadPool->submitJob([&] { mpRenderer->start(); });
  rendererStopped->wait();
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */