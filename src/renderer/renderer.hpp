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

  Renderer(const Settings &settings, ThreadPool &threadPool)
      : mpSettings{&settings},
        mpThreadPool{&threadPool}, mInstance{*mpSettings}, mWindow{*mpSettings},
        mSurface{mInstance, mWindow}, mDevice{mInstance, mSurface} {}

  Renderer(const Renderer &) = delete;

  Renderer(Renderer &&) = default;

  Renderer &operator=(const Renderer &) = delete;

  Renderer &operator=(Renderer &&) = default;

  ~Renderer() = default;

  void start() {
    mWindow.open();
    // Instance instance = std::move(mInstance);
    // mInstance.release();
  }

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