#include "renderer.hpp"

#include "threads.hpp"

namespace neko {

Renderer::Renderer(const Settings &settings, ThreadPool &threadPool)
    : mpSettings{&settings}, mpThreadPool{&threadPool}, mInstance{*mpSettings},
      mWindow{*mpSettings}, mSurface{mInstance, mWindow},
      mDevice{mInstance, mSurface}, mCommandPool{mDevice} {}

Renderer::~Renderer() = default;

void Renderer::start() {
  mWindow.open();
  // Instance instance = std::move(mInstance);
  // mInstance.release();
}

} /* namespace neko */