/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-15 13:09:49
*/

#include <iostream>
#include "render.hpp"

void Render::addHill(Hill h)
{
	hills.push_back(h);
}

void Render::renderHills() const
{
	for (int i = 0; i < hills.size(); i++)
	{
		hills[i].render(_world);
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	_world->clear();
	for (Hill& h : hills)
	{
		h.tick();
	}
}

sf::RenderTexture* Render::getTex() const
{
	return _world;
}