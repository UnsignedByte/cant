/*
* @Author: UnsignedByte
* @Date:   2021-04-11 14:21:25
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-18 21:58:29
*/

#pragma once
#include "ant.hpp"
#include <SFML/System.hpp>


struct Hill
{
public:
	Hill() = default;
	Hill(float x, float y, int antCount, sf::RenderTexture* rt): _pos(x,y), _world(rt)
	{
		for (int i = 0; i < antCount; i++)
		{
			_ants.push_back(Ant(_pos, _world));
		}
	}
	static Hill randomHill(int boundX, int boundY, int antCount, sf::RenderTexture* rt)
	{
		return Hill(utils::rand::rand_01()*boundX, utils::rand::rand_01()*boundY, antCount, rt);
	}

	void tick();

	int antCount() const;

	void render(sf::RenderTexture& win) const;

private:
	std::vector<Ant> _ants; // list of ants
	int _food; // food held in colony
	sf::Vector2f _pos;
	sf::RenderTexture* _world;
};