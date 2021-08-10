#pragma once
#include "Sound.h"

namespace SoundEngine {
	class Effect : public ASound{
	public:
		Effect(const std::string& path);

		void Play(int loops) override;
		void Stop() override;
	private:
		std::shared_ptr<Mix_Chunk> pEffect;
	};
}