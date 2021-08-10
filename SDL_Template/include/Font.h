#pragma once
#include <SDL_ttf.h>
#include "Graphics.h"

namespace GraphicsEngine {
	class Font : public CoreSystem::SingletonMaker<Font>
	{
	public:
		Font(const std::string& path, int size);
		~Font();

		// Draws text at a given position.
		void						DrawText(Maths::IVec2D pos, const std::string& text, GraphicsEngine::Color c = { 255, 255, 255, 255 });
		void						DrawText(int x, int y, const std::string& text, GraphicsEngine::Color c = { 255, 255, 255, 255 }) {
			DrawText(Maths::IVec2D(x, y), text, c);
		}
	private:
		std::shared_ptr<Graphics>	pGfx;
		std::shared_ptr<TTF_Font>	pFont;
	};
}

