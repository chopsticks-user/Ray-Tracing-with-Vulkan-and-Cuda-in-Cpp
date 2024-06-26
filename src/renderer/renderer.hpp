#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "utils.hpp"

#include "basic/instance.hpp"
#include "basic/surface.hpp"
#include "basic/window.hpp"
#include "commands/commands.hpp"
#include "devices/logical_device.hpp"
#include "devices/physical_device.hpp"
#include "devices/queues.hpp"

namespace neko {

class ThreadPool;

class Renderer {
public:
  Renderer() = delete;
  Renderer(const Renderer &) = delete;
  Renderer(Renderer &&) = default;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = default;

  Renderer(const Settings &settings, ThreadPool &threadPool);

  ~Renderer();

  void start();

private:
  const Settings *mpSettings;
  ThreadPool *mpThreadPool;

  Instance mInstance;
  Window mWindow;
  Surface mSurface;
  Device mDevice;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_HPP */