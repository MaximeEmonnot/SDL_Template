#pragma once
#include <queue>
#include "Vec2D.h"
class Mouse
{
private:
	friend class Window;
public:
	enum class EventType {
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
	Mouse() = default;
	~Mouse() = default;

	// Returns the last Event caught.
	Mouse::EventType			Read();

	// Returns mouse position on the X axis.
	int							GetMousePosX() const;
	// Returns mouse position on the Y axis.
	int							GetMousePosY() const;
	// Returns mouse coordonates. Can be used with Vec2D.
	IVec2D						GetMousePos() const;
	// Returns true if Left Mouse Button is currently pressed, false otherwise.
	bool						LeftIsPressed() const;
	// Returns true if Right Mouse Button is currently pressed, false otherwise.
	bool						RightIsPressed() const;

private:
	void						OnMouseMove(int x, int y);
	void						OnLeftPressed();
	void						OnLeftReleased();
	void						OnRightPressed();
	void						OnRightReleased();
	void						OnWheelUp();
	void						OnWheelDown();
	void						TrimBuffer();
	void						Flush();

private:
	bool						leftIsPressed = false;
	bool						rightIsPressed = false;
	int							x = 0;
	int							y = 0;
	static constexpr size_t		sizeBuffer = 4u;
	std::queue<EventType>		buffer;
};