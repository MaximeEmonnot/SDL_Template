#pragma once
#include <SDL.h>
#include "Vec2D.h"
class Mouse
{
private:
	friend class Window;
public:
	enum class Type {
	LPress,
	LRelease,
	RPress,
	RRelease,
	WheelUp,
	WheelDown,
	Move,
	None
	};
public:
	Mouse(class Window& wnd);
	~Mouse() = default;

	Mouse::Type Read();

	Sint32 GetMousePosX();
	Sint32 GetMousePosY();
	Vec2D<Sint32> GetMousePos();
	bool LeftIsPressed();
	bool RightIsPressed();

private:
	void SetType(Type newType);

private:
	SDL_Event& event;
	Type type;
};

