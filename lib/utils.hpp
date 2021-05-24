/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:54
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-23 20:54:49
*/
#pragma once
#include <SFML/Graphics.hpp>

namespace constants
{
	const float movSpeed = 0.01;
}

namespace utils
{
	namespace rand
	{
		float rand_01();

		int urand(int min, int max);
	}
	namespace math
	{

		//Angle class with helper functions; contains a direction and a vector
		struct Angle
		{
		public:
			Angle() = default;

			Angle(float angle)
			{
				_angle = angle;
				updateVec();
			}

			void setAngle(float angle);

			void operator +=(float a);
			void operator +=(Angle a);
			Angle operator +(float a) const;
			Angle operator +(Angle a) const;
			Angle operator -(float a) const;
			Angle operator -(Angle a) const;

			float getAngle() const;

			sf::Vector2f getVec() const;
		private:
			sf::Vector2f _vec;
			float _angle;

			void updateVec();
		};
	}
}

std::ostream& operator<<(std::ostream&, const sf::Vector2f);

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Rect<T> v)
{
	os << "("<<v.top<<", "<<v.left<<", "<<v.width<<", "<<v.height<<")";
	return os;
}

float arfmod(const float, const float);

template <typename A, typename B>
sf::Vector2f arfmod(const sf::Vector2<A>& a, const sf::Vector2<B>& b)
{
	sf::Vector2f j = sf::Vector2f(a);
	sf::Vector2f k = sf::Vector2f(b);
	return sf::Vector2f(arfmod(j.x, k.x), arfmod(j.y, k.y));
}