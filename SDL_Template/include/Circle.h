#pragma once
#include "Rect.h"

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

		bool IsOverlappingRect(const Rect<T>& rect) const {
			return Distance(Vec2D<T>(rect.rect.x, rect.rect.y), pos) < radius ||
				Distance(Vec2D<T>(rect.rect.x + rect.rect.w, rect.rect.y), pos) < radius ||
				Distance(Vec2D<T>(rect.rect.x, rect.rect.y + rect.rect.h), pos) < radius ||
				Distance(Vec2D<T>(rect.rect.x + rect.rect.w, rect.rect.y + rect.h), pos) < radius;
		}

	public:
		Vec2D<T> pos;
		T radius;
	};

	typedef Circle<int> ICircle;
	typedef Circle<float> FCircle;
}