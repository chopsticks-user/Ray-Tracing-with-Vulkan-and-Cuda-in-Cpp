#include "Engine.hpp"

#include "GenericRenderer.hpp"

namespace Neko::Internal
{
    Engine::Engine()
    {
        mpConfigs = std::make_unique<Core::EngineConfigs>("Data/Configs/configs.json");

        mpThreadPool = std::make_unique<Core::ThreadPool>(*mpConfigs);

        auto rendererReady = mpThreadPool->submitJob(
            [&]
            { mpRenderer = std::make_unique<Renderer>(*mpConfigs, *mpThreadPool); });
        rendererReady->wait();
    };

    Engine::~Engine() = default;

    void Engine::start()
    {
        auto rendererStopped = mpThreadPool->submitJob([&]
                                                       { mpRenderer->start(); });
        rendererStopped->wait();
    }

    void Engine::stop() { mpThreadPool->release(); }

} // namespace Neko::Internal