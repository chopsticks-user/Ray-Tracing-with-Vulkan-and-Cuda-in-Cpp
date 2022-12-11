#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "settings.hpp"

#include "renderer/renderer.hpp"
#include "threads/thread_pool.hpp"

#include <iostream>
#include <memory>

namespace neko {

class Engine {
public:
  Engine() {
    mpSettings = std::make_unique<Settings>();

    mpThreadPool = std::make_unique<ThreadPool>(*mpSettings);

    mpRenderer = std::make_unique<Renderer>(*mpSettings, *mpThreadPool);
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
   * !{mThreadPool} is only destroyed when all resources are no longer in
   * !used by all the threads. Therefore, it must be destroyed first to
   * !prevent any thread from unexpectedly being released or using destroyed
   * !resources.
   */
  std::unique_ptr<ThreadPool> mpThreadPool;
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */