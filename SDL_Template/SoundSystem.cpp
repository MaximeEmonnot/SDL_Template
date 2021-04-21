#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

SoundSystem::~SoundSystem()
{
	Mix_CloseAudio();
}

void SoundSystem::PlayOneMusic(int musicIndex, int loops)
{
	for (auto& music : musicList) {
		if (music.GetIndex() == musicIndex) {
			music.Play(loops);
			return;
		}
	}
}

void SoundSystem::PlayAllMusics(int loops)
{
	for (auto& music : musicList) {
		music.Play(loops);
	}
}

void SoundSystem::PlayOneEffect(int effectIndex, int loops)
{
	for (auto& effect : effectList) {
		if (effect.GetIndex() == effectIndex) {
			effect.Play(loops);
			return;
		}
	}
}

void SoundSystem::PlayAllEffects(int loops)
{
	for (auto& effect : effectList) {
		effect.Play(loops);
	}
}

void SoundSystem::PlayAll(int loops)
{
	PlayAllMusics(loops);
	PlayAllEffects(loops);
}

void SoundSystem::StopOneMusic(int musicIndex)
{
	for (auto& music : musicList) {
		if (music.GetIndex() == musicIndex) {
			music.Stop();
			return;
		}
	}
}

void SoundSystem::StopAllMusics()
{
	for (auto& music : musicList) {
		music.Stop();
	}
}

void SoundSystem::StopOneEffect(int effectIndex)
{
	for (auto& effect : effectList) {
		if (effect.GetIndex() == effectIndex) {
			effect.Stop();
			return;
		}
	}
}

void SoundSystem::StopAllEffects()
{
	for (auto& effect : effectList) {
		effect.Stop();
	}
}

void SoundSystem::StopAll()
{
	StopAllMusics();
	StopAllEffects();
}

void SoundSystem::AddSound(const char* path, bool type)
{

	if (type) {
		effectList.emplace_back(path, lastEffectIndex);
		lastEffectIndex++;
	}
	else {
		musicList.emplace_back(path, lastMusicIndex);
		lastMusicIndex++;
	}
}

void SoundSystem::FlushSounds()
{
	FlushMusics();
	FlushSounds();
}

void SoundSystem::FlushMusics()
{
	musicList.clear();
	lastMusicIndex = 0;
}

void SoundSystem::FlushEffects()
{
	effectList.clear();
	lastEffectIndex = 0;
}
