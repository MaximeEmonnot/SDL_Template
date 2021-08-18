#include "ThreadPool.h"
#include <SDL.h>
#include "EngineException.h"

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
				try {
					task();
				}
				catch (const EngineException& e) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.GetType().c_str(), e.GetMessage().c_str(), NULL);
				}
				catch (const std::exception& e) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "STL Exception caught", (std::string("A STL Exception has been caught during Game Routine.\nDetails:\n") + std::string(e.what())).c_str(), NULL);
				}
				catch (...) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unknown error caught", "An unknown error has been caught during Game Routine.", NULL);
				}
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
