#include "engine.hpp"

#include "renderer.hpp"
#include "threads.hpp"

namespace neko {

Engine::Engine() {
  TIMER_START(settingsTimer);
  mpSettings = std::make_unique<Settings>("data/configs/settings.json");
  TIMER_STOP(settingsTimer, "Settings' load time");

  TIMER_START(threadPoolTimer);
  mpThreadPool = std::make_unique<ThreadPool>(*mpSettings);
  TIMER_STOP(threadPoolTimer, "Thread pool's creation time");

  printf("%ld\n", mpThreadPool->threadCount());

  TIMER_START(rendererTimer);
  bool rendererReady;
  mpThreadPool->submitJob(
      [&] {
        mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
      },
      rendererReady);
  TIMER_STOP(rendererTimer, "Renderer's creation time");

  waitTillReady(rendererReady);
};

Engine::~Engine() = default;

void Engine::start() {
  mpThreadPool->submitJob([&] { mpRenderer->start(); });
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */