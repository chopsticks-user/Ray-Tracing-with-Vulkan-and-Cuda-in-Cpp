#ifndef NEKO_THREADS_HPP
#define NEKO_THREADS_HPP

#include "utils.hpp"

#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

namespace neko {

class ThreadPool {
  typedef std::function<void()> Job_T;
  typedef std::unique_lock<std::mutex> MutexLock_T;

public:
  ThreadPool() { initializePool(); }
  ThreadPool(const Settings &settings) {
    initializePool(settings.system.cpuThreadUsage);
  }
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;
  ~ThreadPool() { release(); }

  size_t threadCount() const noexcept { return mThreads.size(); }

  /**
   * @brief
   * ! {neko::waitTillReady} must be called in the same scope where {readyFlag}
   * ! lives
   *
   * @param job
   * @param readyFlag
   */
  void submitJob(const Job_T &job, bool &readyFlag);

  void submitJob(const Job_T &job);

  bool busy();

  const std::map<std::thread::id, u64> &jobStatus() const noexcept {
    return mJobStatus;
  }

  u64 totalSubmittedJob() const noexcept { return mTotalSubmittedJob; }

  void force_release();

  void release();

private:
  std::vector<std::thread> mThreads;
  std::queue<Job_T> mJobs;
  std::mutex mQueueMutex;
  std::condition_variable mMutexCondition;
  bool mShouldTerminate;
  std::map<std::thread::id, u64> mJobStatus;
  u64 mTotalSubmittedJob;

  void initializePool(CPUThreadUsage usageMode = medium);

  static void threadLoop(ThreadPool *pool);
};

void waitTillReady(const std::vector<std::reference_wrapper<bool>> &readyFlags);

void waitTillReady(bool &readyFlag);

} /* namespace neko */

#endif /* NEKO_THREADS_HPP */