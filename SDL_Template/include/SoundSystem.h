#pragma once
#include <SDL_mixer.h>
#include <unordered_set>
#include "EngineException.h"
#include "SingletonMaker.h"
#include "Effect.h"
#include "Music.h"

#define MUSIC 0
#define EFFECT 1

namespace SoundEngine {
	class SoundSystem : public CoreSystem::SingletonMaker<SoundSystem>
	{
	public:
		SoundSystem();
		~SoundSystem();

		void					PlaySound(const std::shared_ptr<ASound>& sound, int loops);

		std::shared_ptr<ASound> ConstructNewSong(const char* path, bool type);

	private:
		void                    AddNewSong(const std::shared_ptr<ASound>& sound);
		void				    FlushSounds();


	private:
		std::unordered_set<std::shared_ptr<ASound>, ASound::Hash> mSoundList;
	};
}