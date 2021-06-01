#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "SDLException.h"

namespace GraphicsEngine {
	class Sprite
	{
	public:
		Sprite();
		Sprite(const char* path);
		Sprite(SDL_Surface* surf, int width, int height);
		Sprite(const Sprite&);

		Sprite& operator= (const Sprite& rhs);
		~Sprite();

		// Initializes the sprite. Needs the Graphics Renderer.
		void			InitSurface(const char* path);

		// Returns the saved Texture.
		SDL_Texture* GetTexture() const;
		// Returns the saved Texture's width.
		int				GetWidth() const;
		// Returns the savec Texture's height.
		int				GetHeight() const;

	private:
		SDL_Texture*    mpTex;
		int				mWidth = 0;
		int				mHeight = 0;
	};
}