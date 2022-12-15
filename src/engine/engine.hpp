#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "utils.hpp"

#include <memory>

namespace neko {

class Renderer;
class ThreadPool;

class Engine {
public:
  Engine(const Engine &) = delete;
  Engine(Engine &&) = default;
  Engine &operator=(const Engine &) = delete;
  Engine &operator=(Engine &&) = default;

  Engine();

  ~Engine();

  void start();

  void stop();

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