#ifndef NEKO_THREADS_HPP
#define NEKO_THREADS_HPP

#include "core.hpp"

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace neko {

class Engine;
class ThreadPool;

class JobPromise {
public:
  friend class ThreadPool;

  bool wait() {
    if (!mFinished) {
      mFinished = mFuture.get();
    }
    return mFinished;
  }

private:
  std::promise<bool> mPromise;
  std::future<bool> mFuture = mPromise.get_future();
  bool mFinished = false;

  void setFlag() { mPromise.set_value(true); }
};

class ThreadPool {
  typedef std::function<void()> Job_T;
  typedef std::unique_lock<std::mutex> MutexLock_T;

public:
  ThreadPool() { initializePool(); }
  ThreadPool(const Configs &settings) {
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
   * ! The caller must ensure that {job} is alive until the worker thread
   * ! finishes using it.
   *
   * @param job
   * @return std::shared_ptr<JobPromise>
   */
  std::shared_ptr<JobPromise> submitJob(const Job_T &job);

  bool busy();

  void force_release();

  void release();

private:
  std::vector<std::thread> mThreads;
  std::queue<Job_T> mJobs;
  std::mutex mQueueMutex;
  std::condition_variable mMutexCondition;
  bool mShouldTerminate;

  void initializePool(CPUThreadUsage usageMode = CPUThreadUsage::medium);

  static void threadLoop(ThreadPool *pool);
};

} /* namespace neko */

#endif /* NEKO_THREADS_HPP */