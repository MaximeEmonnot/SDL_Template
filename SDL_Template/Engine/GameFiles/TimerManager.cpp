#include "TimerManager.h"

void TimerManager::Update()
{
	timer -= pTimer->DeltaTime();
}

bool TimerManager::IsTimerDown() const
{
	return timer <= 0.0f;
}

void TimerManager::ResetTimer(float value)
{
	timer = value;
}

float TimerManager::GetTimer() const
{
	return timer;
}
