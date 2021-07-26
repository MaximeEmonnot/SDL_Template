#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"

namespace GraphicsEngine {
	class Font : public CoreSystem::SingletonMaker<Font>
	{
	public:
		Font(const char* path, int size);
		~Font();

		// Draws text at a given position.
		void        DrawText(Maths::IVec2D pos, const char* text, GraphicsEngine::Color c = { 255, 255, 255, 255 });
		void		DrawText(int x, int y, const char* text, GraphicsEngine::Color c = { 255, 255, 255, 255 }) {
			DrawText(Maths::IVec2D(x, y), text, c);
		}
	private:
		std::shared_ptr<Graphics>	pGfx;
		TTF_Font*					pFont;
	};
}

