/*
* @Author: UnsignedByte
* @Date:   2021-04-13 23:38:32
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 10:05:39
*/
#include "hill.hpp"
#include "render.hpp"
// #include <cstdio>

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
	hill.setPosition(_pos-sf::Vector2f(HILL_SIZE, HILL_SIZE));
	hill.setFillColor(sf::Color(127,127,127,63));
	hill.setOutlineThickness(1);
	hill.setOutlineColor(sf::Color(127,127,127));
	_render->world()->draw(hill);
}

//returns true if hill has died (no energy)
void Hill::tick() //calculate movements for all ants in hill
{
	// make sure there are ants in hill before we do anything
	if (antCount() == 0) return;

	//replenish energy of ants
	for(int i = 0; i < antCount(); i++) {
		// IF within hill, transfer energy
		if (utils::math::dstsq(_pos, _ants[i].getPos()) < HILL_SIZE*HILL_SIZE) {
			_E-= _ants[i].setE();
		}
	}

	// IF chance passed, create a new ant
	// std::cout << "TOTAL POP: "<< antCount() << std::endl;
	if (utils::rand::rand_01() < BIRTH_CHANCE) {
		int i = utils::rand::urand(0,antCount());
		// std::cout << "CREATING NEW ANT FROM SOURCE ID "<< i << " out of " << antCount() << std::endl;
		addAnt(_ants[i].brain());
	}

	// std::cout << _render << std::endl;
	// remove ants if tick() returns true, aka if ant is out of energy
	_ants.erase(
		std::remove_if(_ants.begin(), _ants.end(),
			[](Ant& a)
			{
				a.tick();
				// printf("test\n");
				return !a.alive();
			}
		), _ants.end());
}

float Hill::E() const
{
	float TE = _E;
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

void Hill::addAnt(const Network brain) {
	if (_E > _ant_allocated)
	{
		_ants.push_back(Ant(_pos, _ant_allocated, MAX_STOMACH_SIZE, brain, _render, this));
		_E-=_ant_allocated;
		_ants.back().mutate();
	}
	//  else {
	// 	printf("FAILED CREATING ANT DUE TO LACK OF E\n");
	// }
}