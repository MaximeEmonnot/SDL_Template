#pragma once
#include <SDL_mixer.h>
#include <cassert>
#include <vector>

#define MUSIC 0
#define EFFECT 1

class SoundSystem
{
private:
	class Music {
	public:
		Music(const char* path, int index) {
			this->index = index;
			music = Mix_LoadMUS(path);
			assert(music != NULL);
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
			assert(effect != NULL);
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

	void PlayOneMusic(int musicIndex, int loops);
	void PlayAllMusics(int loops);
	void PlayOneEffect(int effectIndex, int loops);
	void PlayAllEffects(int loops);
	void PlayAll(int loops);
	void StopOneMusic(int soundIndex);
	void StopAllMusics();
	void StopOneEffect(int effectIndex);
	void StopAllEffects();
	void StopAll();

	void AddSound(const char* path, bool type);
	void FlushSounds();
	void FlushMusics();
	void FlushEffects();

private:
	int lastMusicIndex = 0;
	int lastEffectIndex = 0;
	std::vector<SoundSystem::Music> musicList;
	std::vector<SoundSystem::Effect> effectList;
};

