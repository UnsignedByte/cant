/*
* @Author: UnsignedByte
* @Date:   2021-04-11 14:21:25
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-16 23:17:13
*/

#pragma once
#include "ant.hpp"
#include <SFML/System.hpp>


struct Hill
{
public:
	Hill() = default;
	Hill(float x, float y, int antCount): _pos(x,y)
	{
		for (int i = 0; i < antCount; i++)
		{
			_ants.push_back(Ant(_pos));
		}
	}
	static Hill randomHill(int boundX, int boundY, int antCount)
	{
		return Hill(utils::rand::rand_01()*boundX, utils::rand::rand_01()*boundY, antCount);
	}

	void tick();

	int antCount() const;

	void render(sf::RenderTexture& win) const;

private:
	std::vector<Ant> _ants; // list of ants
	int _food; // food held in colony
	sf::Vector2f _pos;
};