#include "Timer.h"

CoreSystem::Timer::Timer()
	:
	mLast(std::chrono::steady_clock::now()),
	deltaTime(0.0f)
{}

void CoreSystem::Timer::Update()
{
	const auto old = mLast;
	mLast = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(mLast - old).count();
}

float CoreSystem::Timer::DeltaTime()
{
	return deltaTime;
}
