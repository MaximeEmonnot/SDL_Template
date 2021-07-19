#pragma once
#include <chrono>
#include "SingletonMaker.h"

namespace CoreSystem {
	class Timer : public SingletonMaker<Timer>
	{
	public:
		Timer();

		void Update();

		//Returns elapsed time since last frame. Can be used in Updates functions. 
		float DeltaTime();

	private:
		float deltaTime;
		std::chrono::steady_clock::time_point mLast;
	};
}