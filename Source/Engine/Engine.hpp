#ifndef HDG_ENGINE_HPP
#define HDG_ENGINE_HPP

#include "CoreInternal.hpp"

namespace Neko::Internal
{
    class Renderer;

    class Engine
    {
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
        std::string projectDirectory;
        std::unique_ptr<Core::EngineConfigs> mpConfigs;
        std::unique_ptr<Renderer> mpRenderer;

        /**
         * @brief
         * !{mThreadPool} should only be destroyed when all resources are no longer in
         * !used by all the threads. Therefore, it must be destroyed first to prevent
         * !any thread from unexpectedly being released or using destroyed resources.
         */
        std::unique_ptr<Core::ThreadPool> mpThreadPool;
    };

} // namespace Neko::Internal

#endif /* HDG_ENGINE_HPP */