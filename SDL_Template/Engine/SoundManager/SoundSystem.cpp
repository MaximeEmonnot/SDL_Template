#include "SoundSystem.h"

SoundEngine::SoundSystem::SoundSystem()
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
		throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Sound System Initialisation.");
}

SoundEngine::SoundSystem::~SoundSystem()
{
	Mix_CloseAudio();
}

void SoundEngine::SoundSystem::PlayOneMusic(int musicIndex, int loops)
{
	for (auto& music : musicList) {
		if (music.GetIndex() == musicIndex) {
			music.Play(loops);
			return;
		}
	}
	throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Music Playing.\nPlease check Music Index.");
}

void SoundEngine::SoundSystem::PlayAllMusics(int loops)
{
	for (auto& music : musicList) {
		music.Play(loops);
	}
}

void SoundEngine::SoundSystem::PlayOneEffect(int effectIndex, int loops)
{
	for (auto& effect : effectList) {
		if (effect.GetIndex() == effectIndex) {
			effect.Play(loops);
			return;
		}
	}
	throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Effect Playing.\nPlease check Effect Index.");
}

void SoundEngine::SoundSystem::PlayAllEffects(int loops)
{
	for (auto& effect : effectList) {
		effect.Play(loops);
	}
}

void SoundEngine::SoundSystem::PlayAll(int loops)
{
	PlayAllMusics(loops);
	PlayAllEffects(loops);
}

void SoundEngine::SoundSystem::StopOneMusic(int musicIndex)
{
	for (auto& music : musicList) {
		if (music.GetIndex() == musicIndex) {
			music.Stop();
			return;
		}
	}

	throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Music Stop.\nPlease check Music Index.");
}

void SoundEngine::SoundSystem::StopAllMusics()
{
	for (auto& music : musicList) {
		music.Stop();
	}
}

void SoundEngine::SoundSystem::StopOneEffect(int effectIndex)
{
	for (auto& effect : effectList) {
		if (effect.GetIndex() == effectIndex) {
			effect.Stop();
			return;
		}
	}
	throw SDLException("SDL Sound System Exception caught", __FILE__, __LINE__, "An error has been caught during Effect Stop.\nPlease check Effect Index.");
}

void SoundEngine::SoundSystem::StopAllEffects()
{
	for (auto& effect : effectList) {
		effect.Stop();
	}
}

void SoundEngine::SoundSystem::StopAll()
{
	StopAllMusics();
	StopAllEffects();
}

void SoundEngine::SoundSystem::AddSound(const char* path, bool type)
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

void SoundEngine::SoundSystem::FlushSounds()
{
	FlushMusics();
	FlushSounds();
}

void SoundEngine::SoundSystem::FlushMusics()
{
	musicList.clear();
	lastMusicIndex = 0;
}

void SoundEngine::SoundSystem::FlushEffects()
{
	effectList.clear();
	lastEffectIndex = 0;
}
