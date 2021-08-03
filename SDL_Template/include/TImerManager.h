#pragma once
#include "Timer.h"

class TimerManager {
public:
	TimerManager() = default;

	void Update();
	bool IsTimerDown() const;
	void ResetTimer(float value);

	float GetTimer() const;

private:
	std::shared_ptr<CoreSystem::Timer> pTimer = CoreSystem::Timer::GetInstance();
	float timer = 0.0f;
};