#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "SDLException.h"
class Sprite
{
public:
	Sprite();
	Sprite(const char* path, SDL_Renderer* renderer);
	Sprite(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height);
	Sprite(const Sprite&);

	Sprite& operator= (const Sprite& rhs);
	~Sprite();

	// Initializes the sprite. Needs the Graphics Renderer.
	void			InitSurface(const char* path, SDL_Renderer* renderer);

	// Returns the saved Texture.
	SDL_Texture*	GetTexture() const;
	// Returns the saved Texture's width.
	int				GetWidth() const;
	// Returns the savec Texture's height.
	int				GetHeight() const;

private:
	SDL_Texture*	tex;
	int				width = 0;
	int				height = 0;
};

