/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:20:16
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-26 17:01:35
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

	Ant(float x, float y, float dir, float se, float ss, Network brain, Render* rt, Hill* hill): _pos(x,y), _dir(utils::math::Angle(dir)), _brain(brain), _render(rt), _E(0), _hill(hill), _stomach_size(ss), _stomach_equil(se)
	{
		// std::cout << brain << std::endl;
	}

	// ant with random angle
	Ant(float x, float y, float se, float ss, Network brain, Render* rt, Hill* hill): Ant(x, y, utils::rand::rand_01()*2*M_PI, se, ss, brain, rt, hill)
	{
	}

	Ant(sf::Vector2f pos, float se, float ss, Network brain, Render* rt, Hill* hill): Ant(pos.x, pos.y, se, ss, brain, rt, hill)
	{
	}

	static void mutate(Ant& a) {
		Network::mutate(a._brain);
		nUtils::RANDOM_MUTATE(a._stomach_size, false);
		nUtils::RANDOM_MUTATE(a._stomach_equil, false);
	}

	void tick();

	float E() const;
	float setE(float);

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
	float _E, _stomach_size, _stomach_equil;
};