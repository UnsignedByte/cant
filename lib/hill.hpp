/*
* @Author: UnsignedByte
* @Date:   2021-04-11 14:21:25
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-26 17:01:47
*/

#pragma once
#include "ant.hpp"
#include "rtneat.hpp"
#include <SFML/System.hpp>

struct Render;

struct Hill
{
public:
	Hill() = default;
	Hill(float x, float y, int antCount, float E, Render* rt, Network brain): _pos(x,y), _render(rt), _E(E), _brain(brain)
	{
		float AllocatedE = _E/antCount;
		for (int i = 0; i < antCount; i++)
		{
			_ants.push_back(Ant(_pos, AllocatedE/2, AllocatedE, _brain, _render, this));
			Ant::mutate(_ants.back());
		}
	}

	static Hill randomHill(int boundX, int boundY, int a, int e, Render* rt)
	{
		return Hill(utils::rand::rand_01()*boundX, utils::rand::rand_01()*boundY, a, e, rt, Network::random());
	}

	static Hill randomHill(int boundX, int boundY, int amin, int amax, int e, Render* rt)
	{
		return randomHill(boundX, boundY, utils::rand::urand(amin, amax), e, rt);
	}

	void tick();

	int antCount() const;

	float E() const;

	void render() const;
	void setRender(Render* r);

private:
	std::vector<Ant> _ants; // list of ants
	sf::Vector2f _pos;
	Render* _render;
	Network _brain;
	float _E;
};