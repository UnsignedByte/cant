/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 16:59:41
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <cmath>

struct Render;

/**
 * Ant class, contains a position and direction
 */
struct Ant
{
public:
	Ant() = default;

	Ant(float x, float y, float dir,  int E, Render* rt): _pos(x,y), _dir(utils::math::Angle(dir)), _render(rt), _E(E)
	{
	}

	// ant with random angle
	Ant(float x, float y, int E, Render* rt): Ant(x, y, utils::rand::rand_01()*2*M_PI, E, rt)
	{
	}

	Ant(sf::Vector2f pos, int E, Render* rt): Ant(pos.x, pos.y, E, rt)
	{
	}

	void tick();

	int E() const;

	bool render(sf::VertexArray& arr, int i) const;

	sf::Vector2f getPos() const;

	utils::math::Angle getAngle() const;
private:
	sf::Vector2f _pos;
	utils::math::Angle _dir;
	Render* _render;
	int _E;
};