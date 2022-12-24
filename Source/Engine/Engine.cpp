#include "Engine.hpp"

#include "Renderer.hpp"

namespace neko {

Engine::Engine() {
  mpConfigs = std::make_unique<EngineConfigs>("Data/Configs/configs.json");

  mpThreadPool = std::make_unique<ThreadPool>(*mpConfigs);

  auto rendererReady = mpThreadPool->submitJob([&] {
    mpRenderer = std::make_unique<Renderer>(*mpConfigs, *mpThreadPool);
  });
  rendererReady->wait();
};

Engine::~Engine() = default;

void Engine::start() {
  auto rendererStopped = mpThreadPool->submitJob([&] { mpRenderer->start(); });
  rendererStopped->wait();
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */