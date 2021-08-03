#include "TextBox.h"

TextBox::TextBox(const Maths::IRect& rect)
	:
	pFont(GraphicsEngine::Font::GetInstance("ttf/arcadeClassic.TTF", 16)),
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	rect(rect)
{
}

void TextBox::Draw(const std::string& text, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor, const GraphicsEngine::Color& backgroundColor)
{
	pGfx->DrawFilledRect(rect, backgroundColor);
	pGfx->DrawRect(rect, boxColor);
	pFont->DrawText(rect.GetVec2DFrom(), text.c_str(), textColor);
}
