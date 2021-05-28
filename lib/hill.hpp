/*
* @Author: UnsignedByte
* @Date:   2021-04-11 14:21:25
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-28 01:12:28
*/

#pragma once
#include "ant.hpp"
#include "rtneat.hpp"
#include <cassert>
#include <SFML/System.hpp>

struct Render;

const float HILL_SIZE = 5.f;
const float MAX_STOMACH_SIZE = 1200;
// chance to create new ant based off current one;
const float BIRTH_CHANCE = 1;
// number of ants at hill creation
const int INITIAL_ANTS = 10;

struct Hill
{
public:
	Hill() = default;
	Hill(float x, float y, float E, float aa, Render* rt, Network brain);

	static Hill randomHill(int boundX, int boundY, int e, Render* rt)
	{
		return Hill(utils::rand::rand_01()*boundX, utils::rand::rand_01()*boundY, e, MAX_STOMACH_SIZE/2, rt, Network::random());
	}

	static Hill clone(const Hill& h, const int e);

	void tick();
	void addAnt(const Network brain);

	int antCount() const;

	float E() const;

	void render() const;

private:
	std::vector<Ant> _ants; // list of ants
	sf::Vector2f _pos;
	Render* _render;
	float _E, _ant_allocated, _color;
	sf::FloatRect _bounds;
};