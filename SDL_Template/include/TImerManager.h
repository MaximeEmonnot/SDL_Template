#pragma once
#include "Timer.h"

class TimerManager {
public:
	TimerManager() = default;

	void Update(float& timer);
	bool IsTimerDown(const float& timer) const;
	void ResetTimer(float& timer, float value) const;

private:

	std::shared_ptr<CoreSystem::Timer> pTimer = CoreSystem::Timer::GetInstance();
};