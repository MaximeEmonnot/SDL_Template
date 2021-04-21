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

	Mouse::EventType			Read();

	int							GetMousePosX() const;
	int							GetMousePosY() const;
	IVec2D						GetMousePos() const;
	bool						LeftIsPressed() const;
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