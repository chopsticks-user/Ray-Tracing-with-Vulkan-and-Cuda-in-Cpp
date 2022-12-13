#include "threads.hpp"

namespace neko {

void ThreadPool::submitJob(const Job_T &job, bool &readyFlag) {
  {
    MutexLock_T lock{mQueueMutex};
    readyFlag = false;
    mJobs.push([&] {
      job();
      readyFlag = true;
    });
    ++mTotalSubmittedJob;
  }
  mMutexCondition.notify_one();
}

void ThreadPool::submitJob(const Job_T &job) {
  {
    MutexLock_T lock{mQueueMutex};
    mJobs.push(job);
    ++mTotalSubmittedJob;
  }
  mMutexCondition.notify_one();
}

bool ThreadPool::busy() {
  bool poolbusy;
  {
    MutexLock_T lock{mQueueMutex};
    poolbusy = mJobs.empty();
  }
  return poolbusy;
}

void ThreadPool::force_release() {
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

void ThreadPool::release() {
  while (!busy()) {
  }
  force_release();
}

void ThreadPool::initializePool(CPUThreadUsage usageMode) {
  auto supportedThreadCount = std::thread::hardware_concurrency();
  if (supportedThreadCount < 4) {
    throw std::runtime_error("std::thread::hardware_concurrency() < 4");
  }
  mShouldTerminate = false;
  mThreads.resize(std::thread::hardware_concurrency() / usageMode);
  for (auto &thread : mThreads) {
    thread = std::thread{ThreadPool::threadLoop, this};
  }
  mTotalSubmittedJob = 0;
}

void ThreadPool::threadLoop(ThreadPool *pool) {
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
      ++pool->mJobStatus[std::this_thread::get_id()];
    }
    job();
  }
}

void waitTillReady(
    const std::vector<std::reference_wrapper<bool>> &readyFlags) {
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

void waitTillReady(bool &readyFlag) {
  while (!readyFlag) {
  }
}

} /* namespace neko */
