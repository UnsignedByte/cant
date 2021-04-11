/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-11 14:10:51
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <cmath>

struct Ant
{
public:
	Ant() = default;

	Ant(float x, float y): _pos(x,y), _dir(utils::rand::rand_01()*2*M_PI)
	{
	}

	Ant(float x, float y, float dir): _pos(x,y), _dir(dir)
	{
	}

	void move();

	sf::CircleShape* render();

	sf::Vector2f getPos() const;

	utils::math::Angle getAngle() const;
private:
	sf::Vector2f _pos;
	utils::math::Angle _dir;
};