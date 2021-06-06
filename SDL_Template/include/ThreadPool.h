#pragma once
#include "SingletonMaker.h"
#include <functional>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <queue>

namespace CoreSystem {
	class ThreadPool : public SingletonMaker<ThreadPool> {
	public:
		ThreadPool(size_t numThreads);
		~ThreadPool();

		void Enqueue(std::function<void()> task);

	private:
		std::vector<std::thread> mThreads;
		std::queue<std::function<void()>> mTasks;
		std::condition_variable mEventVar;
		std::mutex mEventMutex;
		bool mbStopping = false;
	};
}