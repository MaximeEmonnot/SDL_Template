#pragma once
#include "Vec2D.h"

namespace Maths {
	template <typename T>
	class Rect {
	public:
		class Hash {
		public:
			size_t operator()(const Rect& rhs) const {
				std::hash<int> hasher;
				return ((hasher(rhs.rect.x) ^ (hasher(rhs.rect.y) << 1)) >> 1) ^ ((hasher(rhs.rect.w) ^ hasher(rhs.rect.h) << 1) >> 1);
			}
		};

	public:
		Rect()
			:
			rect({ 0, 0, 0, 0 })
		{}
		Rect(T x, T y, T width, T height)
			:
			rect({ (int)x, (int)y, (int)width, (int)height })
		{}
		Rect(Vec2D<T> pos, T width, T height)
			:
			Rect(pos.x, pos.y, width, height)
		{}
		Rect(Vec2D<T> topLeft, Vec2D<T> bottomRight)
			:
			Rect(topLeft, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y)
		{}

		Rect operator+(const Vec2D<T>& rhs) {
			rect.x += rhs.x;
			rect.y += rhs.y;
			return *this;
		}
		void operator+= (const Vec2D<T>& rhs) {
			*this = *this + rhs;
		}

		Rect operator-(const Vec2D<T>& rhs) {
			rect.x -= rhs.x;
			rect.y -= rhs.y;
			return *this;
		}
		void operator-= (const Vec2D<T>& rhs) {
			*this = *this - rhs;
		}

		bool operator==(const Rect& rhs) const {
			return rect.x == rhs.rect.x && rect.y == rhs.rect.y && rect.w == rhs.rect.w && rect.h == rhs.rect.h;
		}
		bool operator!=(const Rect& rhs) const {
			return !(this == rhs);
		}

		Vec2D<T> GetVec2DFrom() const {
			return Vec2D<T>(rect.x, rect.y);
		}
		Vec2D<T> GetCenterOfRect() const {
			return Vec2D<T>(rect.x + rect.w / 2, rect.y + rect.h / 2);
		}

		bool ContainsVec2D(const Vec2D<T>& pos) const {
			return rect.x <= pos.x
				&& (rect.x + rect.w) >= pos.x
				&& rect.y <= pos.y
				&& (rect.y + rect.h) >= pos.y;
		}

	public:
		SDL_Rect rect;
	};
	typedef Rect<int> IRect;
	typedef Rect<float> FRect;
} 
