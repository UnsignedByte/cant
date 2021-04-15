/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-15 10:36:53
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <cmath>

/**
 * Ant class, contains a position and direction
 */
struct Ant
{
public:
	Ant() = default;

	Ant(sf::Vector2f pos): _pos(pos), _dir(utils::math::Angle(utils::rand::rand_01()*2*M_PI))
	{
	}

	Ant(float x, float y): _pos(x,y), _dir(utils::math::Angle(utils::rand::rand_01()*2*M_PI))
	{
	}

	Ant(float x, float y, float dir): _pos(x,y), _dir(utils::math::Angle(dir))
	{
	}

	void move();

	void render(sf::VertexArray& arr, int i) const;

	sf::Vector2f getPos() const;

	utils::math::Angle getAngle() const;
private:
	sf::Vector2f _pos;
	utils::math::Angle _dir;
};