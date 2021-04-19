/*
* @Author: UnsignedByte
* @Date:   2021-04-13 23:38:32
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-18 21:42:09
*/
#include "hill.hpp"
#define HILL_SIZE 5.f

int Hill::antCount() const
{
	return _ants.size();
}

void Hill::render(sf::RenderTexture& win) const
{
	sf::VertexArray ants(sf::Triangles, antCount()*3);
	for(int i = 0; i < antCount(); i++)
	{
		_ants[i].render(ants, i*3);
	}
	win.draw(ants);
	sf::CircleShape hill(HILL_SIZE);
	hill.setPosition(_pos-sf::Vector2f(HILL_SIZE/2, HILL_SIZE/2));
	hill.setFillColor(sf::Color(148, 102, 28, 255));
	win.draw(hill);
}

void Hill::tick() //calculate movements for all ants in hill
{
	for (int i = 0; i < _ants.size(); i++){
		_ants[i].move();
	}
}