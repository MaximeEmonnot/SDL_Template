#pragma once
#include "Sound.h"

namespace SoundEngine {
	class Music : public ASound{
	public:
		Music(const char* path);
		~Music();

		void Play(int loops) override;
		void Stop() override;
	private:
		Mix_Music* pMusic = NULL;
	};
}