#ifndef NEKO_THREAD_POOL_HPP
#define NEKO_THREAD_POOL_HPP

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

  void submitJob(const Job_T &job, bool &readyFlag) {
    {
      MutexLock_T lock{mQueueMutex};
      readyFlag = false;
      mJobs.push([&] {
        job();
        readyFlag = true;
      });
    }
    mMutexCondition.notify_one();
  }

  void submitJob(const Job_T &job) {
    {
      MutexLock_T lock{mQueueMutex};
      mJobs.push(job);
    }
    mMutexCondition.notify_one();
  }

  bool busy() {
    bool poolbusy;
    {
      MutexLock_T lock{mQueueMutex};
      poolbusy = mJobs.empty();
    }
    return poolbusy;
  }

  const std::map<std::thread::id, u64> &jobStatus() const noexcept {
    return mJobStatus;
  }

  void force_release() {
    {
      MutexLock_T lock{mQueueMutex};
      mShouldTerminate = true;
    }
    mMutexCondition.notify_all();
    for (auto &activeThread : mThreads) {
      activeThread.join();
    }
    mThreads.clear();
  }

  void release() {
    while (!busy()) {
    }
    force_release();
  }

private:
  std::vector<std::thread> mThreads;
  std::queue<Job_T> mJobs;
  std::mutex mQueueMutex;
  std::condition_variable mMutexCondition;
  bool mShouldTerminate;
  std::map<std::thread::id, u64> mJobStatus;

  void initializePool(CPUThreadUsage usageMode = medium) {
    auto supportedThreadCount = std::thread::hardware_concurrency();
    if (supportedThreadCount < 4) {
      throw std::runtime_error("std::thread::hardware_concurrency() < 4");
    }
    mShouldTerminate = false;
    mThreads.resize(std::thread::hardware_concurrency() / usageMode);
    for (auto &thread : mThreads) {
      thread = std::thread{ThreadPool::threadLoop, this};
    }
  }

  static void threadLoop(ThreadPool *pool) {
    while (true) {
      Job_T job;
      {
        MutexLock_T lock{pool->mQueueMutex};
        pool->mMutexCondition.wait(lock, [pool] {
          return !pool->mJobs.empty() || pool->mShouldTerminate;
        });
        if (pool->mShouldTerminate) {
          return;
        }
        job = pool->mJobs.front();
        pool->mJobs.pop();
        pool->mJobStatus[std::this_thread::get_id()]++;
      }
      job();
    }
  }
};

inline void
waitTillReady(const std::vector<std::reference_wrapper<bool>> &readyFlags) {
  bool stopFlag = false;
  u64 flagCount = readyFlags.size();
  while (!stopFlag) {
    for (u64 iFlag = 0; iFlag < flagCount; ++iFlag) {
      if (readyFlags[iFlag] == false) {
        iFlag = 0;
      }
    }
    stopFlag = true;
  }
}

inline void waitTillReady(bool &readyFlag) {
  while (!readyFlag) {
  }
}

} /* namespace neko */

#endif /* NEKO_THREAD_POOL_HPP */