#include "engine.hpp"

#include "renderer.hpp"
#include "threads.hpp"

#include <iostream>
#include <typeinfo>

namespace neko {

void Engine::initRenderer() {
  mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
}

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

  volatile bool rendererReady;
  mpThreadPool->submitJob(
      [&] {
        mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
      },
      rendererReady);
  waitTillReady(rendererReady);
};

Engine::~Engine() = default;

void Engine::start() {
  mpThreadPool->submitJob([&] { mpRenderer->start(); });
}

void Engine::stop() { mpThreadPool->release(); }

} /* namespace neko */