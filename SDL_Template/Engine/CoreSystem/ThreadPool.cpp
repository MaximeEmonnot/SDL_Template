#include "ThreadPool.h"

CoreSystem::ThreadPool::ThreadPool(size_t numThreads)
{
	for (size_t i = 0; i < numThreads; ++i) {
		mThreads.emplace_back([&] {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(mEventMutex);
					mEventVar.wait(lock, [&] { return mbStopping || !mTasks.empty(); });
					if (mbStopping) break;

					task = std::move(mTasks.front());
					mTasks.pop();
				}
				task();
			}
			});
	}
}

CoreSystem::ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(mEventMutex);
		mbStopping = true;
	}
	mEventVar.notify_all();
	
	for (auto& thread : mThreads) {
		thread.join();
	}
}

void CoreSystem::ThreadPool::Enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(mEventMutex);
		mTasks.emplace(std::move(task));
	}
	mEventVar.notify_one();
}
