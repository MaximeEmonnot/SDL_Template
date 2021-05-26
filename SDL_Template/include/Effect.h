#pragma once
#include "Sound.h"

namespace SoundEngine {
	class Effect : public ASound{
	public:
		Effect(const char* path);
		~Effect();

		void Play(int loops) override;
		void Stop() override;
	private:
		Mix_Chunk* effect;
	};
}