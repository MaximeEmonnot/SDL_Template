#pragma once
template <typename T>
class Vec2D {
public:
	Vec2D()
	{
		x = y = 0;
	}
	Vec2D(T x, T y) 
		: 
		x(x), 
		y(y) 
	{}

public:
	T x;
	T y;
};

typedef Vec2D<int> IVec2D;