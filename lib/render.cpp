/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-18 23:10:30
*/

#include <iostream>
#include "render.hpp"
#include <SFML/OpenGL.hpp>

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
	sf::Vector2f offset(D-A,S-W);
	updateView(offset);
}

sf::Sprite Render::getDrawn() const
{
	return sf::Sprite(_world.getTexture(), sf::IntRect(_view));
}

unsigned int Render::getGLHandle() const
{
	return _world.getTexture().getNativeHandle();
}

void Render::updateView(sf::Vector2f transposition) {
	transposition*=constants::movSpeed;
	transposition.x*=_view.width;
	transposition.y*=_view.height;
	_view.left+=transposition.x;
	_view.top+=transposition.y;
	// std::cout << view.getSize() << std::endl;
}

int Render::width() const
{
	return WIDTH;
}

int Render::height() const
{
	return HEIGHT;
}