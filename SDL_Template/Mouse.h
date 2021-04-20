#pragma once
#include <SDL.h>
#include <queue>
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
	void OnMouseMove();
	void OnLeftPressed();
	void OnLeftReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelUp();
	void OnWheelDown();
	void TrimBuffer();

private:
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	SDL_Event& event;

	static constexpr size_t sizeBuffer = 4u;
	std::queue<Type> buffer;
};

