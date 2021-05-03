#pragma once
#include <SDL_mixer.h>
#include <vector>
#include "SDLException.h"
#include "Effect.h"
#include "Music.h"

#define MUSIC 0
#define EFFECT 1

namespace SoundEngine {
	class SoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();

		// Plays one music given an index. Throws an exception if no corresponding music is found.
		void								PlayOneMusic(int musicIndex, int loops);
		// Plays all musics registered in the system.
		void								PlayAllMusics(int loops);
		// Plays one effect given an index. Throws an exception if no corresponding effect is found.
		void								PlayOneEffect(int effectIndex, int loops);
		// Plays all effects registered in the system.
		void								PlayAllEffects(int loops);
		// Plays all sounds registered in the system (musics and effects).
		void								PlayAll(int loops);
		// Stops one music given an index. Throws an exception if no corresponding music is found.
		void								StopOneMusic(int musicIndex);
		// Stops all musics registered in the system.
		void								StopAllMusics();
		// Stops one effect given an index. Throws an exception if no corresponding effect is found.
		void								StopOneEffect(int effectIndex);
		// Stops all effects registered in the system.
		void								StopAllEffects();
		// Stops all sounds registered in the system (musics and effects).
		void								StopAll();

		// Adds a new song. Path must be relative (ex : "music\\[name].wav"). Type can be MUSIC or EFFECT.
		void								AddSound(const char* path, bool type);

	private:
		void								FlushSounds();
		void								FlushMusics();
		void								FlushEffects();

	private:
		int									lastMusicIndex = 0;
		int									lastEffectIndex = 0;
		std::vector<Music>					musicList;
		std::vector<Effect>					effectList;
	};
}
