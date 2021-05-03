#include "Timer.h"

CoreSystem::Timer::Timer()
{
	last = std::chrono::steady_clock::now();
}

float CoreSystem::Timer::DeltaTime()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	return std::chrono::duration<float>(last - old).count();
}
