#ifndef VKH_UTILITY_HPP
#define VKH_UTILITY_HPP

#include <chrono>
#include <iostream>
#include <random>

namespace vkh {
using mic = std::chrono::microseconds;
using mil = std::chrono::milliseconds;
using sec = std::chrono::seconds;
using min = std::chrono::minutes;
using hr = std::chrono::hours;

enum timer_behavior { silence, announce };

template <typename DurationType = mic, timer_behavior when_destroyed = silence>
class Timer {
private:
  std::chrono::steady_clock::time_point start;
  std::string time_unit;

public:
  Timer() {
    if (std::is_same_v<DurationType, mic>)
      this->time_unit = "μs";
    else if (std::is_same_v<DurationType, mil>)
      this->time_unit = "ms";

    else if (std::is_same_v<DurationType, sec>)
      this->time_unit = "s";

    else if (std::is_same_v<DurationType, min>)
      this->time_unit = "mins";

    else if (std::is_same_v<DurationType, hr>)
      this->time_unit = "hrs";
    else
      static_assert("Unkown time unit.");

    this->start = std::chrono::steady_clock::now();
  }

  ~Timer() {
    auto stop = std::chrono::steady_clock::now();
    if (when_destroyed == announce)
      std::cout
          << "Time elapsed: "
          << std::chrono::duration_cast<DurationType>(stop - start).count()
          << ' ' << this->time_unit << "\n";
  }

  double current() const {
    return std::chrono::duration_cast<DurationType>(
               std::chrono::steady_clock::now() - start)
        .count();
  }

  void reset() { start = std::chrono::steady_clock::now(); }
};

inline double rand(double min, double max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min, max);
  return dis(gen);
}

template <typename ItTp>
inline void rand(double min, double max, ItTp it_begin, ItTp it_end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min, max);
  while (it_begin != it_end)
    *(it_begin++) = dis(gen);
}
} // namespace vkh

#endif /* VKH_UTILITY_HPP */