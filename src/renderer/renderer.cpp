#include "renderer.hpp"

#include "threads.hpp"

namespace neko {

Renderer::Renderer(const Configs &settings, ThreadPool &threadPool)
    : mpSettings{&settings}, mpThreadPool{&threadPool}, mInstance{*mpSettings},
      mWindow{*mpSettings}, mSurface{mInstance, mWindow}, mDevice{mInstance,
                                                                  mSurface},
      mCommandPool{mDevice}, mSwapchain{settings, mSurface, mDevice},
      mDepthBuffer{settings, mDevice, mSwapchain} {}

Renderer::~Renderer() = default;

void Renderer::start() {
  mWindow.open();
  // Instance instance = std::move(mInstance);
  // mInstance.release();
}

} /* namespace neko */