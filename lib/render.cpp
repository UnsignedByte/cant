/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 14:41:36
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
	_world.clear();
	for (int i = 0; i < hills.size(); i++)
	{
		hills[i].render(_world);
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	// remove ants if tick() returns true, aka if ant is out of energy
	hills.erase(
		std::remove_if(hills.begin(), hills.end(),
			[](Hill& a)
			{
				a.tick();
				return a.E() <= 0;
			}
		), hills.end());
	
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
	return _bounds.width;
}

int Render::height() const
{
	return _bounds.height;
}

sf::IntRect Render::bounds() const
{
	return _bounds;
}

int Render::E() const
{
	int TE = _E;
	for(const Hill& h : hills)
	{
		TE += h.E();
	}
	return TE;
}