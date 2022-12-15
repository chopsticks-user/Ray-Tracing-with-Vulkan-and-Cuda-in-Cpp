#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "utils.hpp"

#include "../renderer/renderer.hpp"
#include "../threads/threads.hpp"

#include <memory>

namespace neko {

class Engine {
public:
  Engine() {
    TIMER_START(settingsTimer);
    mpSettings = std::make_unique<Settings>("configs/settings.json");
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

  Engine(const Engine &) = delete;

  Engine(Engine &&) = default;

  Engine &operator=(const Engine &) = delete;

  Engine &operator=(Engine &&) = default;

  ~Engine() = default;

  void start() {
    mpThreadPool->submitJob([&] { mpRenderer->start(); });
  }

  void stop() { mpThreadPool->release(); }

private:
  std::unique_ptr<Settings> mpSettings;
  std::unique_ptr<Renderer> mpRenderer;

  /**
   * @brief
   * !{mThreadPool} should only be destroyed when all resources are no longer in
   * !used by all the threads. Therefore, it must be destroyed first to prevent
   * !any thread from unexpectedly being released or using destroyed resources.
   */
  std::unique_ptr<ThreadPool> mpThreadPool;
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */