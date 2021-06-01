#include "Effect.h"

SoundEngine::Effect::Effect(const char* path)
	:
	pEffect(Mix_LoadWAV(path))
{
	this->path = path;
	if (pEffect == NULL)
		throw SDLException("SDL Sound System Exception caught", __FILE__, "An error has been caught during SoundEffect Initialisation.\nPlease check file path.", __LINE__);
}

SoundEngine::Effect::~Effect()
{
	Mix_FreeChunk(pEffect);
}

void SoundEngine::Effect::Play(int loops)
{
	Mix_PlayChannel(-1, pEffect, loops);
}

void SoundEngine::Effect::Stop()
{
	Mix_Pause(-1);
}
