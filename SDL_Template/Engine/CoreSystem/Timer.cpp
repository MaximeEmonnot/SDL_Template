#include "Timer.h"
#include "Vec2D.h"

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

Maths::IVec2D CoreSystem::Timer::GetCurrentTime()
{
	time_t now = time(0);

	tm ltm;
	
	localtime_s(&ltm, &now);

	return Maths::IVec2D(ltm.tm_hour, ltm.tm_min);
}

bool CoreSystem::Timer::IsNightTime()
{
	return (GetCurrentTime().x >= 18 && GetCurrentTime().y >= 30) || (GetCurrentTime().x > 18) || (GetCurrentTime().x <= 6 && GetCurrentTime().y < 30);
}

float CoreSystem::Timer::DeltaTime()
{
	return deltaTime;
}
