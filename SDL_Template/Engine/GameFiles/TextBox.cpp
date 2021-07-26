#include "TextBox.h"

TextBox::TextBox(GraphicsEngine::Font& font, const Maths::IRect& rect)
	:
	font(font),
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	rect(rect)
{
}

void TextBox::Draw(const std::string& text, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor)
{
	font.DrawText(rect.GetVec2DFrom(), text.c_str(), textColor);
	pGfx->DrawRect(rect, boxColor);
}
