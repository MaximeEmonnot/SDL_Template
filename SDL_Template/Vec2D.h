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

	inline T GetX() const {
		return x;
	}
	inline T GetY() const {
		return y;
	}

private:
	T x;
	T y;
};

typedef Vec2D<int> IVec2D;