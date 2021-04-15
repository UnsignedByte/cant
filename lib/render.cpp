/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-14 23:45:46
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
		hills[i].render(_window);
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	for (Hill& h : hills)
	{
		h.tick();
	}
}