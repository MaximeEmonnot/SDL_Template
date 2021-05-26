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
void SoundEngine::SoundSystem::PlaySound(const std::shared_ptr<ASound>& sound, int loops)
{
	AddNewSong(sound);
	sound->Play(loops);
}

std::shared_ptr<SoundEngine::ASound> SoundEngine::SoundSystem::ConstructNewSong(const char* path, bool type)
{
	auto itr = std::find_if(soundList.begin(), soundList.end(), [&](const std::shared_ptr<ASound>& sound) { return sound->GetPath() == path; });
	if (itr != soundList.end())
		return *itr;
	if (type) {
		std::shared_ptr<ASound> newSound =	std::make_shared<Effect>(path);
		return newSound;
	}
	else {
		std::shared_ptr<ASound> newSound = std::make_shared<Music>(path);
		return newSound;
	}
}

void SoundEngine::SoundSystem::AddNewSong(const std::shared_ptr<ASound>& sound)
{
	auto itr = std::find_if(soundList.begin(), soundList.end(), [&](const std::shared_ptr<ASound>& testSound) { return sound->GetPath() == testSound->GetPath(); });
	if (itr == soundList.end())
		soundList.insert(sound);
}

void SoundEngine::SoundSystem::FlushSounds()
{
	soundList.clear();
}