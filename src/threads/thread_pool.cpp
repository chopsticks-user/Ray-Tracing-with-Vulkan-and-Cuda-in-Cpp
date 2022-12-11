// #include "thread_pool.hpp"

// namespace neko {

// ThreadPool::ThreadPool() : mShouldTerminate{false} {
//   auto threadCount = std::thread::hardware_concurrency();
//   mThreads.resize(threadCount);
//   for (auto &thread : mThreads) {
//     thread = std::thread{ThreadPool::threadLoop, this};
//   }
// }

// ThreadPool::~ThreadPool() {
//   {
//     MutexLock_T lock{mQueueMutex};
//     mShouldTerminate = true;
//   }
//   mMutexCondition.notify_all();
//   for (auto &activeThread : mThreads) {
//     activeThread.join();
//   }
//   mThreads.clear();
// }

// void ThreadPool::submitJob(const Job_T &job) {
//   {
//     MutexLock_T lock{mQueueMutex};
//     mJobs.push(job);
//   }
//   mMutexCondition.notify_one();
// }

// void ThreadPool::threadLoop(ThreadPool *pool) {
//   while (true) {
//     Job_T job;
//     {
//       MutexLock_T lock{pool->mQueueMutex};
//       pool->mMutexCondition.wait(lock, [pool] {
//         return !pool->mJobs.empty() || pool->mShouldTerminate;
//       });
//       if (pool->mShouldTerminate) {
//         return;
//       }
//       job = pool->mJobs.front();
//       pool->mJobs.pop();
//     }
//     job();
//   }
// }

// } /* namespace neko */