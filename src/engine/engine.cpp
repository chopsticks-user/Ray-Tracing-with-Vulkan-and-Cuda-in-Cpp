#include "engine.hpp"

#include "renderer.hpp"
#include "threads.hpp"

namespace neko {

Engine::Engine(const std::string &settingsFilePath) {
  TIMER_START(settingsTimer);
  if (settingsFilePath.length() == 0) {
    mpSettings = std::make_unique<Settings>();
  } else {
    mpSettings = std::make_unique<Settings>(settingsFilePath);
  }
  TIMER_INVOKE(settingsTimer, "Settings' load time");

  TIMER_START(threadPoolTimer);
  mpThreadPool = std::make_unique<ThreadPool>(*mpSettings);
  TIMER_INVOKE(threadPoolTimer, "Thread pool's creation time");

  mpRendererReady = mpThreadPool->submitJob([&] {
    mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
  });
  // rendererReady->wait();
};

Engine::~Engine() = default;

void Engine::start() {
  mpRendererReady->wait();
  auto rendererStopped = mpThreadPool->submitJob([&] { mpRenderer->start(); });
  rendererStopped->wait();
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */