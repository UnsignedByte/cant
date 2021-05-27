/*
* @Author: UnsignedByte
* @Date:   2021-04-11 14:21:25
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 09:54:44
*/

#pragma once
#include "ant.hpp"
#include "rtneat.hpp"
#include <SFML/System.hpp>

struct Render;

const float HILL_SIZE = 5.f;
const float MAX_STOMACH_SIZE = 2400;
// chance to create new ant based off current one;
const float BIRTH_CHANCE = 0.05;
// number of ants at hill creation
const int INITIAL_ANTS = 5;

struct Hill
{
public:
	Hill() = default;
	Hill(float x, float y, float E, float aa, float r, Render* rt, Network brain): _pos(x,y), _render(rt), _E(E), _ant_allocated(aa), _reserve(r)
	{
		// _ant_allocated = 0.01;
		for (int i = 0; i < INITIAL_ANTS; i++) {
			addAnt(brain, 1);
		}
	}

	static Hill randomHill(int boundX, int boundY, int e, Render* rt)
	{
		return Hill(utils::rand::rand_01()*boundX, utils::rand::rand_01()*boundY, e, utils::rand::urandf(MAX_STOMACH_SIZE/2,MAX_STOMACH_SIZE), utils::rand::urandf(0, e), rt, Network::random());
	}

	void tick();
	void addAnt(const Network brain, bool force = 0);

	int antCount() const;

	float E() const;

	void render() const;
	void setRender(Render* r);

private:
	std::vector<Ant> _ants; // list of ants
	sf::Vector2f _pos;
	Render* _render;
	float _E, _reserve, _ant_allocated;
};