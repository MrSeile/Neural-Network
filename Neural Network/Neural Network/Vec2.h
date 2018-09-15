#pragma once

namespace nn
{
	template<typename T>
	class Vec2
	{
	public:
		union
		{
			struct
			{
				T x, y;
			};
			struct
			{
				T min, max;
			};
		};

		Vec2(T x, T y) : x(x), y(y) {}
		Vec2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

		// Add
		friend Vec2 operator+(Vec2 left, const Vec2& rigth)
		{
			return Vec2(left.x + rigth.x, left.y + rigth.y);
		}

		friend Vec2 operator+(Vec2 left, const float& rigth)
		{
			return Vec2(left.x + rigth, left.y + rigth);
		}

		Vec2& operator+=(const Vec2& rigth)
		{
			this->x += rigth.x;
			this->y += rigth.y;

			return *this;
		}

		Vec2& operator+=(const float& rigth)
		{
			this->x += rigth;
			this->y += rigth;

			return *this;
		}

		// Substract
		friend Vec2 operator-(Vec2 left, const Vec2& rigth)
		{
			return Vec2(left.x - rigth.x, left.y - rigth.y);
		}

		friend Vec2 operator-(Vec2 left, const float& rigth)
		{
			return Vec2(left.x - rigth, left.y - rigth);
		}

		Vec2& operator-=(const Vec2& rigth)
		{
			this->x -= rigth.x;
			this->y -= rigth.y;

			return *this;
		}

		Vec2& operator-=(const float& rigth)
		{
			this->x -= rigth;
			this->y -= rigth;

			return *this;
		}

		// Multiply
		friend Vec2 operator*(Vec2 left, const Vec2& rigth)
		{
			return Vec2(left.x * rigth.x, left.y * rigth.y);
		}

		friend Vec2 operator*(Vec2 left, const float& rigth)
		{
			return Vec2(left.x * rigth, left.y * rigth);
		}

		Vec2& operator*=(const Vec2& rigth)
		{
			this->x *= rigth.x;
			this->y *= rigth.y;

			return *this;
		}

		Vec2& operator*=(const float& rigth)
		{
			this->x *= rigth;
			this->y *= rigth;

			return *this;
		}

		// Divide
		friend Vec2 operator/(Vec2 left, const Vec2& rigth)
		{
			return Vec2(left.x / rigth.x, left.y / rigth.y);
		}

		friend Vec2 operator/(Vec2 left, const float& rigth)
		{
			return Vec2(left.x / rigth, left.y / rigth);
		}

		Vec2& operator/=(const Vec2& rigth)
		{
			this->x /= rigth.x;
			this->y /= rigth.y;

			return *this;
		}

		Vec2& operator/=(const float& rigth)
		{
			this->x /= rigth;
			this->y /= rigth;

			return *this;
		}
	};
}
