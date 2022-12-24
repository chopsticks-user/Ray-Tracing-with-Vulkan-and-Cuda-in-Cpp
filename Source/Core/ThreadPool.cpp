#include "ThreadPool.hpp"

#include "EngineConfigs.hpp"

namespace Neko {

ThreadPool::ThreadPool() { initializePool(CPUThreadUsage::medium); }

ThreadPool::ThreadPool(const EngineConfigs &configs) {
  initializePool(configs.system.cpuThreadUsage);
}

std::shared_ptr<JobPromise> ThreadPool::submitJob(const Job_T &job) {
  std::shared_ptr<JobPromise> jobReady;
  {
    MutexLock_T lock{mQueueMutex};
    jobReady = std::make_shared<JobPromise>();
    mJobs.push([=] {
      job();
      jobReady->setFlag();
    });
  }
  mMutexCondition.notify_one();
  return jobReady;
}

bool ThreadPool::busy() {
  bool poolbusy;
  {
    MutexLock_T lock{mQueueMutex};
    poolbusy = mJobs.empty();
  }
  return poolbusy;
}

void ThreadPool::forceRelease() {
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
  forceRelease();
}

void ThreadPool::initializePool(CPUThreadUsage usageMode) {
  auto supportedThreadCount = std::thread::hardware_concurrency();
  if (supportedThreadCount < 4) {
    throw std::runtime_error("std::thread::hardware_concurrency() < 4");
  }
  mShouldTerminate = false;
  u64 threadCount =
      std::thread::hardware_concurrency() / static_cast<u64>(usageMode);
  threadCount = threadCount > 2 ? threadCount : 2;
  mThreads.resize(threadCount);
  for (auto &thread : mThreads) {
    thread = std::thread{ThreadPool::threadLoop, this};
  }
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
    }
    job();
  }
}

} /* namespace Neko */
