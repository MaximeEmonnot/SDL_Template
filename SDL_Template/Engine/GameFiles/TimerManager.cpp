#include "TimerManager.h"

void TimerManager::Update(float& timer)
{
	timer -= pTimer->DeltaTime();
}

bool TimerManager::IsTimerDown(const float& timer) const
{
	return timer <= 0.0f;
}

void TimerManager::ResetTimer(float& timer, float value) const
{
	timer = value;
}
