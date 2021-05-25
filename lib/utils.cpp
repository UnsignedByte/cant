/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 00:55:07
*/
#include <random>
#include <cassert>
#include <SFML/System.hpp>
#include "utils.hpp"


namespace utils {
	namespace rand {
		std::random_device random_device;
		std::mt19937 random_engine(random_device());
		std::uniform_real_distribution<float> random_distribution(0.0, 1.0);
		std::normal_distribution<float> normdist(0.0, 1.0);

		float rand_01() {
			return random_distribution(random_engine);
		}

		float norm() {
			return normdist(random_engine);
		}

		int urand(int min, int max) {
			return static_cast<int>(utils::rand::rand_01() * (max - min) + min);
		}
	}
	namespace math {
		float sigmoid(float x) {
			return 1/(1+std::exp(-x));
		}

		void Angle::operator+=(float a) {
			setAngle(_angle + a);
		}

		void Angle::operator+=(Angle a) {
			setAngle(_angle + a.getAngle());
		}

		Angle Angle::operator+(float a) const {
			return Angle(a + _angle);
		}

		Angle Angle::operator+(Angle a) const {
			return Angle(a.getAngle() + _angle);
		}

		Angle Angle::operator-(float a) const {
			return Angle(_angle - a);
		}

		Angle Angle::operator-(Angle a) const {
			return Angle(_angle - a.getAngle());
		}

		void Angle::setAngle(float angle) {
			_angle = arfmod(angle, (float)M_PI*2);
		}

		float Angle::getAngle() const {
			return _angle;
		}

		sf::Vector2f Angle::getVec() const {
			return {std::cos(_angle), std::sin(_angle)};
		}
	}

	// Credit to https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	sf::Color HS_vec_to_RGBA(sf::Vector2f hs_vec) {
		sf::Color rgba;

		unsigned int h;

		uint8_t region, remainder, p, q, t, s, v;

		// Always use the max saturation value
		s = 255;

		// Take magnitude of vector (between 0 and inf) and map it into the range [0, 255]
		v = static_cast<uint8_t>(255. * (1 - std::exp(-0.1 * (hs_vec.x * hs_vec.x + hs_vec.y * hs_vec.y))));

		// Find the hue, region and remainder
		h = static_cast<unsigned int>(std::atan2(hs_vec.y, hs_vec.x) * 128. / M_PI);
		region = h / 43;
		remainder = (h - (region * 43)) * 6;

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region) {
			case 0:
				rgba.r = v;
				rgba.g = t;
				rgba.b = p;
				break;
			case 1:
				rgba.r = q;
				rgba.g = v;
				rgba.b = p;
				break;
			case 2:
				rgba.r = p;
				rgba.g = v;
				rgba.b = t;
				break;
			case 3:
				rgba.r = p;
				rgba.g = q;
				rgba.b = v;
				break;
			case 4:
				rgba.r = t;
				rgba.g = p;
				rgba.b = v;
				break;
			default:
				rgba.r = v;
				rgba.g = p;
				rgba.b = q;
				break;
		}

		rgba.a = 255;
		return rgba;
	}
}

std::ostream &operator<<(std::ostream &os, const sf::Vector2f &v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

float arfmod(float a, float b) {
	assert(b != 0);
	return std::fmod(std::fmod(a, b) + b, b);
}

int arimod(int a, int b) {
	assert(b != 0);
	return std::fmod(std::fmod(a, b) + b, b);
}