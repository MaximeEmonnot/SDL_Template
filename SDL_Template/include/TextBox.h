#pragma once
#include "Font.h"
#include "Graphics.h"

class TextBox {
public:
	TextBox(const std::string& text, const Maths::IRect& rect);

	void Draw(GraphicsEngine::Font& font);

private:
	std::shared_ptr<GraphicsEngine::Graphics> pGfx;
	Maths::IRect rect;
	std::string text;
};