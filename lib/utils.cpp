/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-23 20:52:05
*/
#include <random>
#include <cassert>
#include <SFML/System.hpp>
#include "utils.hpp"

namespace utils
{
	namespace rand
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(0.0, 1.0);

		float rand_01()
		{
			return dist(mt);
		}

		int urand(int min, int max){
			return static_cast<int>(utils::rand::rand_01()*(max-min)+min);
		}
	}
	namespace math
	{
		void Angle::operator +=(float a)
		{
			_angle+=a;
			updateVec();
		}

		void Angle::operator +=(Angle a)
		{
			_angle+=a.getAngle();
			updateVec();
		}

		Angle Angle::operator +(float a) const
		{
			return Angle(a+_angle);
		}

		Angle Angle::operator +(Angle a) const
		{
			return Angle(a.getAngle()+_angle);
		}

		Angle Angle::operator -(float a) const
		{
			return Angle(_angle-a);
		}

		Angle Angle::operator -(Angle a) const
		{
			return Angle(_angle-a.getAngle());
		}

		void Angle::setAngle(float angle)
		{
			_angle = angle;
			updateVec();
		}

		float Angle::getAngle() const
		{
			return _angle;
		}

		sf::Vector2f Angle::getVec() const
		{
			return _vec;
		}
		
		void Angle::updateVec()
		{
			_vec.x = std::cos(_angle);
			_vec.y = std::sin(_angle);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f v)
{
	os << "("<<v.x<<", "<<v.y<<")";
	return os;
}

float arfmod(const float a, const float b)
{
	assert(b != 0);
	return std::fmod(std::fmod(a,b)+b, b);
}