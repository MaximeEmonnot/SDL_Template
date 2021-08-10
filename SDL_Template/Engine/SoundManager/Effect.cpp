#include "Effect.h"

SoundEngine::Effect::Effect(const std::string& path)
{
	this->path = path;
	pEffect.reset(Mix_LoadWAV(path.c_str()), Mix_FreeChunk);
	if (pEffect == NULL)
		throw EngineException("SDL Sound System Exception caught", __FILE__, "An error has been caught during SoundEffect Initialisation.\nPlease check file path.", __LINE__);
}

void SoundEngine::Effect::Play(int loops)
{
	Mix_PlayChannel(-1, pEffect.get(), loops);
}

void SoundEngine::Effect::Stop()
{
	Mix_Pause(-1);
}
