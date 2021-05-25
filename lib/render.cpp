/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 18:05:08
*/

#include <iostream>
#include "render.hpp"
#include <SFML/OpenGL.hpp>

void Render::addHill(Hill h)
{
	h.setRender(this);
	hills.push_back(h);
}

void Render::render()
{
	_world.clear();


	//update food image with new colors
	for(int i = 0; i < _bounds.height; i++)
	{
		for (int j = 0; j < _bounds.width; j++)
		{
			_foodImg.setPixel(j, i, utils::HSVec2RGB(_food[i*_bounds.width+j]));
			// _foodImg.setPixel(j, i, sf::Color(255,0,0));
			// printf("test\n");
		}
	}


	// printf("test\n");

	_foodTexture.loadFromImage(_foodImg);
	_foodSprite.setTexture(_foodTexture, true);
	_foodSprite.setPosition(0,0);

	_world.draw(_foodSprite);

	// printf("test\n");

	for (int i = 0; i < hills.size(); i++)
	{
		hills[i].render();
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	for(int i = 0; i < _bounds.height; i++)
	{
		for (int j = 0; j < _bounds.width; j++)
		{
			_food[i*_bounds.width+j]*=0.998f;
		}
	}

	// printf("test\n");

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

sf::RenderTexture* Render::world()
{
	return &_world;
}


std::vector<sf::Vector2f>& Render::food()
{
	return _food;
}