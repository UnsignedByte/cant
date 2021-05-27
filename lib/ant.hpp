/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 11:05:57
*/
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "rtneat.hpp"
#include <cmath>

struct Render;
struct Hill;

const int AGE = 9000; // lifespan of ant in frames

const float ANT_SIZE = 3.f;

/**
 * Ant class, contains a position and direction
 */
struct Ant
{
public:
	Ant() = default;

	Ant(float x, float y, float dir, float se, float ss, Network brain, Render* rt, Hill* hill): _pos(x,y), _dir(dir), _brain(brain), _render(rt), _E(se), _hill(hill), _stomach_size(ss), _stomach_equil(se)
	{
		// std::cout << "CREATED: " << this->_brain << std::endl;
	}

	// ant with random angle
	Ant(float x, float y, float se, float ss, Network brain, Render* rt, Hill* hill): Ant(x, y, utils::rand::rand_01()*2*M_PI, se, ss, brain, rt, hill)
	{
	}

	Ant(sf::Vector2f pos, float se, float ss, Network brain, Render* rt, Hill* hill): Ant(pos.x, pos.y, se, ss, brain, rt, hill)
	{
	}

	void mutate() {
		_brain.mutate();
		nUtils::RANDOM_MUTATE(_stomach_equil, false, _stomach_size);
	}

	void tick();

	float E() const;
	float fullness() const;
	float setE();

	bool alive() const;

	bool render(sf::VertexArray& arr, int i) const;
	Render* render();
	Network brain() const;

	sf::Vector2f getPos() const;

	float getAngle() const;
private:
	sf::Vector2f _pos;
	Render* _render;
	Hill* _hill;
	Network _brain;
	float _E, _dir, _stomach_size, _stomach_equil;
	int _age = AGE;
};