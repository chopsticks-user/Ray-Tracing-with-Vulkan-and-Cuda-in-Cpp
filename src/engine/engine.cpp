#include "engine.hpp"

#include "renderer.hpp"
#include "threads.hpp"

namespace neko {

Engine::Engine() {
  TIMER_START(settingsTimer);
  mpSettings = std::make_unique<Settings>();
  TIMER_INVOKE(settingsTimer, "Settings' load time");

  TIMER_START(threadPoolTimer);
  mpThreadPool = std::make_unique<ThreadPool>(*mpSettings);
  TIMER_INVOKE(threadPoolTimer, "Thread pool's creation time");

  printf("%ld\n", mpThreadPool->threadCount());

  TIMER_START(rendererTimer);
  mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
  TIMER_INVOKE(rendererTimer, "Renderer's creation time");

  // waitTillReady(rendererReady);
};

Engine::~Engine() = default;

void Engine::start() {
  mpThreadPool->submitJob([&] { mpRenderer->start(); });
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */