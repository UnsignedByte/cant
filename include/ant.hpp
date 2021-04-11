#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"


struct Ant
{
	sf::Vector2f pos;
	double dir;

	Ant() = default;

	Ant(double x, double y, double dir): pos(x,y), dir(dir)
	{
	}

	void move()
	{
		pos+=sf::Vector2f(rand_01()-.5f, rand_01()-.5f);
	}

	sf::CircleShape* render();
};