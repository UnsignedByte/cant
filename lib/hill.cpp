/*
* @Author: UnsignedByte
* @Date:   2021-04-13 23:38:32
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 23:27:39
*/
#include "hill.hpp"
#include "render.hpp"
// #include <cstdio>

const float HILL_SIZE = 5.f;
const int DIRS[16] = {1, 1, 1, 0, 1, -1, 0, 1, 0, -1, -1, 1, -1, 0, -1, -1};

int Hill::antCount() const
{
	return _ants.size();
}

void Hill::render() const
{
	sf::FloatRect bounds = sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f(_render->world()->getSize()));
	int C = antCount();
	sf::VertexArray ants(sf::Triangles, C*3*9);
	for(int i = 0; i < antCount(); i++)
	{
		if (_ants[i].render(ants, i*3))
		{
			for(int j = 0; j < 8; j++)
			{
				sf::Vector2f offset(DIRS[2*j]*bounds.width, DIRS[2*j+1]*bounds.height);
				ants[C*3] = ants[i*3].position+offset;
				ants[C*3+1] = ants[i*3+1].position+offset;
				ants[C*3+2] = ants[i*3+2].position+offset;
				if (bounds.contains(ants[C*3].position) || bounds.contains(ants[C*3+1].position) || bounds.contains(ants[C*3+2].position))
				{
					// draw second copy of triangle if ant wraps over limits
					C++;
				}
			}
		}
	}
	// printf("%d\n", C);
	ants.resize(C*3);
	_render->world()->draw(ants);
	sf::CircleShape hill(HILL_SIZE);
	hill.setPosition(_pos-sf::Vector2f(HILL_SIZE/2, HILL_SIZE/2));
	hill.setFillColor(sf::Color(148, 102, 28, 255));
	_render->world()->draw(hill);
}

//returns true if hill has died (no energy)
void Hill::tick() //calculate movements for all ants in hill
{
	// std::cout << _render << std::endl;
	// remove ants if tick() returns true, aka if ant is out of energy
	_ants.erase(
		std::remove_if(_ants.begin(), _ants.end(),
			[](Ant& a)
			{
				a.tick();
				// printf("test\n");
				return a.E() < 0;
			}
		), _ants.end());
}

int Hill::E() const
{
	int TE = _E;
	for (const Ant& a : _ants)
	{
		TE+=a.E();
	}
	return TE;
}

void Hill::setRender(Render* r)
{
	_render = r;
}