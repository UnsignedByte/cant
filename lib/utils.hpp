/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:54
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-18 22:00:08
*/
#pragma once

namespace constants
{
	const float movSpeed = 0.05;
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