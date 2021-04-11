/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-11 12:31:54
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"


struct Ant
{
public:
	Ant() = default;

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