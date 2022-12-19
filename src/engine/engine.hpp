#ifndef NEKO_ENGINE_HPP
#define NEKO_ENGINE_HPP

#include "core.hpp"

#include <memory>

#include <functional>

namespace neko {

class Renderer;
class ThreadPool;
class JobPromise;

class Engine {
public:
  Engine(const Engine &) = delete;
  Engine(Engine &&) = default;
  Engine &operator=(const Engine &) = delete;
  Engine &operator=(Engine &&) = default;

  Engine(const std::string &settingsFilePath = "");

  ~Engine();

  void start();

  void stop();

private:
  std::string projectDirectory;
  std::unique_ptr<Configs> mpSettings;
  std::unique_ptr<Renderer> mpRenderer;
  std::shared_ptr<JobPromise> mpRendererReady;

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