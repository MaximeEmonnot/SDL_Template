#pragma once
#include <SDL_mixer.h>
#include <vector>
#include "SDLException.h"

#define MUSIC 0
#define EFFECT 1

class SoundSystem
{
private:
	class Exception : public SDLException {
	public:
		Exception(const std::string& file, unsigned int line, const std::string& note) noexcept
			:
			SDLException(file, line, note)
		{}
		inline std::string GetType() const noexcept override {
			return "SDL Sound System Exception caught";
		}
	};
private:
	class Music {
	public:
		Music(const char* path, int index) {
			this->index = index;
			music = Mix_LoadMUS(path);
			if (music == NULL)
				throw Exception(__FILE__, __LINE__, "An error has been caught during Music Initialisation.\nPlease check file path.");
		}
		~Music() {
			Mix_FreeMusic(music);
		}
		void Play(int loops) {
			Mix_PlayMusic(music, loops);
		}
		void Stop() {
			Mix_PauseMusic();
		}
		inline int GetIndex() const { return index; }
	private:
		int index = 0;
		Mix_Music* music;
	};
	class Effect {
	public:
		Effect(const char* path, int index) {
			this->index = index;
			effect = Mix_LoadWAV(path);
			if (effect == NULL)
				throw Exception(__FILE__, __LINE__, "An error has been caught during SoundEffect Initialisation.\nPlease check file path.");
		}
		~Effect() {
			Mix_FreeChunk(effect);
		}
		void Play(int loops) {
			Mix_PlayChannel(-1, effect, loops);
		}
		void Stop() {
			Mix_Pause(-1);
		}
		inline int GetIndex() const { return index; }
	private:
		int index = 0;
		Mix_Chunk* effect;
	};

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
	std::vector<SoundSystem::Music>		musicList;
	std::vector<SoundSystem::Effect>	effectList;
};

