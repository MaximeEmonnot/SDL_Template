#pragma once
#include <chrono>
class Timer
{
public:
	Timer();

	//Returns elapsed time since last call. Can be used in Updates functions. 
	float DeltaTime();

private:
	std::chrono::steady_clock::time_point last;
};

