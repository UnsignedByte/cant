/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-16 23:33:21
*/

#include <iostream>
#include "render.hpp"
// #include <SFML/OpenGL.hpp>

void Render::addHill(Hill h)
{
	hills.push_back(h);
}

void Render::renderHills()
{
	for (int i = 0; i < hills.size(); i++)
	{
		hills[i].render(_world);
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	_world.clear();
	for (Hill& h : hills)
	{
		h.tick();
	}
	sf::Vector2f offset(A-D,S-W);
	updateView(offset);
}

sf::Sprite Render::getDrawn() const
{
	return sf::Sprite(_world.getTexture());
}

void Render::updateView(sf::Vector2f& transposition) {
	sf::View view = _world.getView();
	view.move(transposition);
	std::cout << view.getSize() << std::endl;
	_world.setView(view);
}