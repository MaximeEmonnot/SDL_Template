#pragma once
#include <SDL_rect.h>
template <typename T>
class Vec2D {
public:
	Vec2D() = default;
	Vec2D(T x, T y) 
		: 
		x(x), 
		y(y) 
	{}
	Vec2D& operator= (const Vec2D& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	template <typename T2>
	explicit Vec2D (Vec2D<T2> v) 
		:
		Vec2D((T)v.x, (T)v.y)
	{}


	Vec2D operator-() {
		x *= -1;
		y *= -1;
		return *this;
	}

	Vec2D operator+(Vec2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	void operator+=(Vec2D& rhs) {
		*this = *this + rhs;
	}
	Vec2D operator-(Vec2D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	void operator-=(Vec2D& rhs) {
		*this = *this - rhs;
	}
	Vec2D operator*(T scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
	void operator*=(T scale) {
		*this = *this * scale;
	}
	Vec2D operator/(T scale) {
		T invScale = 1 / scale;
		return *this * invScale;
	}
	void operator/=(T scale) {
		*this = *this / scale;
	}
	
	bool operator==(Vec2D& rhs) {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(Vec2D& rhs) {
		return !(*this == rhs);
	}

public:
	T x = (T)0;
	T y = (T)0;
};

typedef Vec2D<int> IVec2D;
typedef Vec2D<float> FVec2D;

template<typename T>
SDL_Rect operator+(SDL_Rect rect,const Vec2D<T>& rhs) {
	rect.x += rhs.x;
	rect.y += rhs.y;
	return rect;
}
template<typename T>
void operator+=(SDL_Rect& rect, const Vec2D<T>& rhs)
{
	rect = rect + rhs;
}
template<typename T>
SDL_Rect operator-(SDL_Rect rect, const Vec2D<T>& rhs) {
	rect.x -= rhs.x;
	rect.y -= rhs.y;
	return rect;
}
template<typename T>
void operator-=(SDL_Rect& rect, const Vec2D<T>& rhs) {
	rect = rect - rhs;
}
template<typename T>
SDL_Rect VecToRect(SDL_Rect rect, const Vec2D<T>& rhs) {
	rect.x = rhs.x;
	rect.y = rhs.y;
	return rect;
}
template <typename T>
SDL_Rect VecToRect(const Vec2D<T>& topLeft, const Vec2D<T>& rightBottom) {
	return { topLeft.x, topLeft.y, rightBottom.x - topLeft.x, rightBottom.y - topLeft.y };
}
template<typename T>
SDL_Rect VecToRect(const Vec2D<T>& topLeft, int width, int height) {
	return { topLeft.x, topLeft.y, width, height };
}