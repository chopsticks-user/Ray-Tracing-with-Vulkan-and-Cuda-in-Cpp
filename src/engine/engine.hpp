#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "settings.hpp"

#include "renderer/renderer.hpp"
#include "threads/thread_pool.hpp"

#include <cmath>
#include <iostream>
#include <memory>

namespace neko {

class Engine {
public:
  Engine() {
    mSettings = std::make_unique<Settings>();
    mThreadPool = std::make_unique<ThreadPool>(*mSettings);
    mRenderer = std::make_unique<Renderer>(*mSettings);
  };

  Engine(const Engine &) = delete;

  Engine(Engine &&) = default;

  Engine &operator=(const Engine &) = delete;

  Engine &operator=(Engine &&) = default;

  ~Engine() = default;

  void start() {
    std::cout << mThreadPool->threadCount() << '\n';
    mThreadPool->submitJob([&] { mRenderer->start(); });
  };

  void stop() { mThreadPool->release(); }

private:
  std::unique_ptr<Settings> mSettings;
  std::unique_ptr<Renderer> mRenderer;

  // !{mThreadPool} must be destroyed last
  std::unique_ptr<ThreadPool> mThreadPool;
};

} /* namespace neko */

#endif /* NEKO_ENGINE_HPP */