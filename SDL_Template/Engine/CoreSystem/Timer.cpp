#include "Timer.h"

CoreSystem::Timer::Timer()
	:
	mLast(std::chrono::steady_clock::now())
{}

float CoreSystem::Timer::DeltaTime()
{
	const auto old = mLast;
	mLast = std::chrono::steady_clock::now();
	return std::chrono::duration<float>(mLast - old).count();
}
