/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:54
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 23:56:37
*/
#pragma once

#include <SFML/Graphics.hpp>

namespace constants {
	const float movSpeed = 0.01f;
}

namespace utils {
	namespace rand {
		/**
		 * Return a random float in the range [0, 1).
		 */
		float rand_01();
		float norm();
		int binom(int, float);

		/**
		 * Return a random integer in the range [min, max)
		 */
		int urand(int min, int max);
	}
	namespace math {
		float sigmoid(float);

		/**
		 * Angle class with helper functions; contains a float direction and a computable unit vector
		 */
		struct Angle {
		public:
			Angle() = default;

			Angle(float angle) {
				setAngle(angle);
			}

			void setAngle(float angle);

			void operator+=(float a);

			void operator+=(Angle a);

			Angle operator+(float a) const;

			Angle operator+(Angle a) const;

			Angle operator-(float a) const;

			Angle operator-(Angle a) const;

			[[nodiscard]]
			float getAngle() const;

			[[nodiscard]]
			sf::Vector2f getVec() const;

		private:
			float _angle;
		};

		sf::Vector2f polar2Cartesian(const float);
		sf::Vector2f polar2Cartesian(const float, const float);
	}

	/**
	 * Convert a vector <hue, saturation> to an sf::Color
	 */
	sf::Color HS_vec_to_RGBA(sf::Vector2f hs_vec);
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const sf::Vector2<T> &v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const sf::Rect<T> v) {
	os << "(" << v.top << ", " << v.left << ", " << v.width << ", " << v.height << ")";
	return os;
}

/**
 * Return the arithmetic modulo operation on two integers
 */
int arimod(int, int);

/**
 * Return the arithmetic modulo operation on two floats
 */
float arfmod(float, float);

/**
 * Return the element-wise arithmetic modulo operation on two Vec2s
 */
template<typename A, typename B>
sf::Vector2f arfmod(const sf::Vector2<A> &a, const sf::Vector2<B> &b) {
	// sf::Vector2f j = sf::Vector2f(a);
	// sf::Vector2f k = sf::Vector2f(b);
	return sf::Vector2f(arfmod((float)a.x, (float)b.x), arfmod((float)a.y, (float)b.y));
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> matrix)
{
	for(int i = 0; i < matrix.size(); i++){
		os << matrix[i] << " ";
	}
	return os << '\b';
}