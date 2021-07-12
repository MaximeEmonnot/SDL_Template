#include "TextBox.h"

TextBox::TextBox(const std::string& text, const Maths::IRect& rect)
	:
	pGfx(GraphicsEngine::Graphics::GetInstance()),
	rect(rect),
	text(text)
{
}

void TextBox::Draw(GraphicsEngine::Font& font, const GraphicsEngine::Color& textColor, const GraphicsEngine::Color& boxColor)
{
	font.DrawText(rect.GetVec2DFrom(), text.c_str(), textColor);
	pGfx->DrawRect(rect, boxColor);
}
