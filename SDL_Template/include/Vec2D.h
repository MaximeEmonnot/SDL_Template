#pragma once
#include <SDL_rect.h>
#include <cmath>

namespace Maths {
	template <typename T>
	class Vec2D {
	public:
		Vec2D() = default;
		Vec2D(T x, T y)
			:
			x(x),
			y(y)
		{}
		Vec2D(Vec2D pos0, Vec2D pos1)
			:
			x(pos1.x - pos0.x),
			y(pos1.y - pos0.y)
		{
		}
		Vec2D& operator= (const Vec2D& rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
		template <typename T2>
		explicit Vec2D(Vec2D<T2> v)
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

		float GetLength() const {
			return sqrtf(x * x + y * y);
		}

	public:
		T x = (T)0;
		T y = (T)0;
	};

	typedef Vec2D<int> IVec2D;
	typedef Vec2D<float> FVec2D;

	template<typename T>
	float Distance(const Vec2D<T>& lhs, const Vec2D<T>& rhs) {
		Vec2D<T> v(lhs, rhs);
		return v.GetLength();
	}
}