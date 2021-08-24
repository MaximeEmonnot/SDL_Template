#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Font.h"
#include <string>

class Button {
public:
	Button(Maths::IVec2D pos, std::string text, std::function<void(int&)> func);
	~Button();

	bool OnClick(Maths::IVec2D mousePos);
	bool OnHover(Maths::IVec2D mousePos) const;

	void ComputeFunction(int& output) const;

	void Draw(GraphicsEngine::Color buttonColor);
	
	bool GetState();

private:
	bool bIsClicked = false;
	Maths::IRect rect;
	std::string text;
	std::function<void(int&)> function;
};