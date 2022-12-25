#include "Surface.hpp"
#include "Instance.hpp"
#include "Window.hpp"

namespace Neko
{

    Surface::Surface(const Instance &crInstance, const Window &crWindow)
        : mpcInstance{&crInstance}
    {
        if (glfwCreateWindowSurface(*crInstance, *crWindow, nullptr, &mSurface) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface.");
        }
    }

    Surface::Surface(Surface &&rhs) noexcept
        : mpcInstance{std::move(rhs.mpcInstance)}, mSurface{std::exchange(
                                                       rhs.mSurface,
                                                       VK_NULL_HANDLE)} {}

    Surface &Surface::operator=(Surface &&rhs) noexcept
    {
        release();
        mpcInstance = std::move(rhs.mpcInstance);
        mSurface = std::exchange(rhs.mSurface, VK_NULL_HANDLE);
        return *this;
    }

    void Surface::release() noexcept
    {
        if (mSurface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(**mpcInstance, mSurface, nullptr);
            mSurface = VK_NULL_HANDLE;
        }
    }

} // namespace Neko