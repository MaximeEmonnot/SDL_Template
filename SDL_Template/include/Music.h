#pragma once
#include "Sound.h"

namespace SoundEngine {
	class Music : public ASound{
	public:
		Music(const std::string& path);

		void Play(int loops) override;
		void Stop() override;
	private:
		std::shared_ptr<Mix_Music> pMusic;
	};
}