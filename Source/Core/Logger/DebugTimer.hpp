#ifndef HDG_CORE_LOGGER_DEBUGTIMER_HPP
#define HDG_CORE_LOGGER_DEBUGTIMER_HPP

#include "Common/Common.hpp"

#include <chrono>

#define TIMER_START(timer_name)        \
    auto timer_name = Neko::DebugTimer \
    {                                  \
        Neko::TimeUnit::milliseconds   \
    }

#define TIMER_INVOKE(timer_name, note) \
    timer_name.invoke([](float x) { printf("%s: %f ms\n", note, x); })

namespace Neko::Core
{
    namespace Enum
    {
        enum class TimeUnit
        {
            microseconds = 1'000'000,
            milliseconds = 1'000,
            seconds = 1,
        };

    } // namespace Neko::Core::Enum

    class DebugTimer
    {
        using Clock_T = std::chrono::steady_clock;
        using TimeUnit_T = std::chrono::microseconds;
        using DurationValue_T = f32;

    public:
        DebugTimer(const DebugTimer &) = delete;
        DebugTimer(DebugTimer &&) = delete;
        DebugTimer &operator=(const DebugTimer &) = delete;
        DebugTimer &operator=(DebugTimer &&) = delete;

        DebugTimer() = default;

        explicit DebugTimer(void (*func)(DurationValue_T))
            : mStart{Clock_T::now()}, mInvokeFunc{func} {}

        DebugTimer(
            Enum::TimeUnit unit, void (*func)(DurationValue_T) = [](float) {})
            : mStart{Clock_T::now()}, mInvokeFunc{func},
              mConvValue{static_cast<DurationValue_T>(unit)}
        {
        }

        ~DebugTimer() { invoke(); }

        void setInvokeFunc(void (*func)(DurationValue_T) = [](float) {})
        {
            mInvokeFunc = func;
        }

        DurationValue_T now() const noexcept { return getDurationTillNow(); }

        void invoke() const noexcept { mInvokeFunc(getDurationTillNow()); }

        void invoke(void (*func)(DurationValue_T)) { func(getDurationTillNow()); }

        void reset() noexcept { mStart = Clock_T::now(); }

    private:
        std::chrono::time_point<Clock_T> mStart = Clock_T::now();
        void (*mInvokeFunc)(DurationValue_T) = [](float) {};
        DurationValue_T mConvValue = 1.0f;

        DurationValue_T getDurationTillNow() const noexcept
        {
            return static_cast<DurationValue_T>(
                       std::chrono::duration_cast<TimeUnit_T>(Clock_T::now() - mStart)
                           .count()) /
                   mConvValue;
        }
    };

} /* namespace Neko::Core */

#endif /* HDG_CORE_LOGGER_DEBUGTIMER_HPP */