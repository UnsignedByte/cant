/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-11 12:26:35
*/
#include <random>
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
		void Angle::setAngle(float angle)
		{
			_angle = angle;
			updateVec();
		}

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