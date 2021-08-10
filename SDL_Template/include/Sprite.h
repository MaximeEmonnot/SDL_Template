#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "DColors.h"
#include "EngineException.h"
#include <memory>

namespace GraphicsEngine {
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(const std::string& path);
		Sprite(std::shared_ptr<SDL_Surface> surf, int width, int height);
		Sprite(const Sprite&);

		Sprite& operator= (const Sprite& rhs);
		~Sprite() = default;

		void							BlendColor(const GraphicsEngine::Color& c);

		// Returns the saved Texture.
		std::shared_ptr<SDL_Texture>    GetTexture() const;
		// Returns the saved Texture's width.
		int								GetWidth() const;
		// Returns the savec Texture's height.
		int								GetHeight() const;

	private:
		std::shared_ptr<SDL_Texture>    mpTex;
		int								mWidth = 0;
		int								mHeight = 0;
	};
}