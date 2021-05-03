#pragma once
#include "Vec2D.h"

namespace Maths {
	template <typename T>
	class Circle {
	public:
		Circle(Vec2D<T> pos, T radius)
			:
			pos(pos),
			radius(radius)
		{}
		Circle(T x, T y, T radius)
			:
			pos(Vec2D<T>(x, y)),
			radius(radius)
		{}

		bool operator==(const Circle& rhs) const {
			return pos == rhs.pos && radius == rhs.radius;
		}
		bool operator!=(const Circle& rhs) const {
			return !(*this == rhs);
		}
	public:
		Vec2D<T> pos;
		T radius;
	};

	typedef Circle<int> ICircle;
	typedef Circle<float> FCircle;

	template<typename T>
	bool RectIsOverlappedByCircle(const SDL_Rect& rect, const Circle<T> circle) {
		return Distance(Vec2D<T>(rect.x, rect.y), circle.pos) < circle.radius ||
			Distance(Vec2D<T>(rect.x + rect.w, rect.y), circle.pos) < circle.radius ||
			Distance(Vec2D<T>(rect.x, rect.y + rect.h), circle.pos) < circle.radius ||
			Distance(Vec2D<T>(rect.x + rect.w, rect.y + rect.h), circle.pos) < circle.radius;
	}
}