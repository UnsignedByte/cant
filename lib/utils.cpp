/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 19:02:24
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
		std::uniform_real_distribution<float> udist(0.0, 1.0);
		std::normal_distribution<float> normdist(0.0, 1.0);

		float rand_01()
		{
			return udist(mt);
		}

		float norm()
		{
			return normdist(mt);
		}

		int urand(int min, int max){
			return static_cast<int>(utils::rand::rand_01()*(max-min)+min);
		}
	}
	namespace math
	{
		float sigmoid(float x)
		{
			return 1/(1+std::exp(-x));
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

	// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	sf::Color HSVec2RGB(const sf::Vector2f& HSVec)
	{
		sf::Color rgb;
		unsigned int h;
    unsigned char region, remainder, p, q, t, s, v;

    s = 255; // always saturation max
    v = HSVec.x*HSVec.x+HSVec.y*HSVec.y; // x^2+y^2
    v = 255*(1-std::exp(-0.1*v));

    h = std::atan2(HSVec.y, HSVec.x) * 128 / M_PI;
    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v;
            break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q;
            break;
    }

    // printf("%d, %d, %d\n", rgb.r, rgb.g, rgb.b);
    return rgb;
	}
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v)
{
	os << "("<<v.x<<", "<<v.y<<")";
	return os;
}

float arfmod(const float a, const float b)
{
	assert(b != 0);
	return std::fmod(std::fmod(a,b)+b, b);
}

int arimod(const int a, const int b)
{
	assert(b != 0);
	return std::fmod(std::fmod(a,b)+b, b);
}