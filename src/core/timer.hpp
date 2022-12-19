#ifndef NEKO_CORE_TIMER_HPP
#define NEKO_CORE_TIMER_HPP

#include "defines.hpp"

#include <chrono>

#define TIMER_START(timer_name)                                                \
  auto timer_name = neko::ScopedTimer { neko::TimeUnit::milliseconds }

#define TIMER_INVOKE(timer_name, note)                                         \
  timer_name.invoke([](float x) { printf("%s: %f ms\n", note, x); })

namespace neko {

enum TimeUnit {
  microseconds = 1'000'000,
  milliseconds = 1'000,
  seconds = 1,
};

class ScopedTimer {
  using Clock_T = std::chrono::steady_clock;
  using TimeUnit_T = std::chrono::microseconds;
  using DurationValue_T = f32;

public:
  ScopedTimer(const ScopedTimer &) = delete;
  ScopedTimer(ScopedTimer &&) = delete;
  ScopedTimer &operator=(const ScopedTimer &) = delete;
  ScopedTimer &operator=(ScopedTimer &&) = delete;

  ScopedTimer() = default;

  explicit ScopedTimer(void (*func)(DurationValue_T))
      : mStart{Clock_T::now()}, mInvokeFunc{func} {}

  ScopedTimer(
      TimeUnit unit, void (*func)(DurationValue_T) = [](float) {})
      : mStart{Clock_T::now()}, mInvokeFunc{func},
        mConvValue{static_cast<DurationValue_T>(unit)} {}

  ~ScopedTimer() { invoke(); }

  void setInvokeFunc(void (*func)(DurationValue_T) = [](float) {}) {
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

  DurationValue_T getDurationTillNow() const noexcept {
    return static_cast<DurationValue_T>(
               std::chrono::duration_cast<TimeUnit_T>(Clock_T::now() - mStart)
                   .count()) /
           mConvValue;
  }
};

} /* namespace neko */

#endif /* NEKO_CORE_TIMER_HPP */