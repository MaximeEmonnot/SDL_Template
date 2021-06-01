#pragma once
#include <chrono>
#include "SingletonMaker.h"

namespace CoreSystem {
	class Timer : public SingletonMaker<Timer>
	{
	public:
		Timer();

		//Returns elapsed time since last call. Can be used in Updates functions. 
		float DeltaTime();

	private:
		std::chrono::steady_clock::time_point mLast;
	};
}