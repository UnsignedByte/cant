/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 21:56:57
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "rtneat.hpp"
#include <cmath>

struct Render;
struct Hill;

/**
 * Ant class, contains a position and direction
 */
struct Ant
{
public:
	Ant() = default;

	Ant(float x, float y, float dir,  int E, Network brain, Render* rt, Hill* hill): _pos(x,y), _dir(utils::math::Angle(dir)), _brain(brain), _render(rt), _E(E), _hill(hill)
	{
		// std::cout << brain << std::endl;
	}

	// ant with random angle
	Ant(float x, float y, int E, Network brain, Render* rt, Hill* hill): Ant(x, y, utils::rand::rand_01()*2*M_PI, E, brain, rt, hill)
	{
	}

	Ant(sf::Vector2f pos, int E, Network brain, Render* rt, Hill* hill): Ant(pos.x, pos.y, E, brain, rt, hill)
	{
	}

	void tick();

	int E() const;

	bool render(sf::VertexArray& arr, int i) const;
	Render* render();

	sf::Vector2f getPos() const;

	utils::math::Angle getAngle() const;
private:
	sf::Vector2f _pos;
	utils::math::Angle _dir;
	Render* _render;
	Hill* _hill;
	Network _brain;
	int _E;
};