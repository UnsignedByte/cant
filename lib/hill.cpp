/*
* @Author: UnsignedByte
* @Date:   2021-04-13 23:38:32
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-28 00:29:00
*/
#include "hill.hpp"
#include "render.hpp"
// #include <cstdio>

const int DIRS[16] = {1, 1, 1, 0, 1, -1, 0, 1, 0, -1, -1, 1, -1, 0, -1, -1};

Hill::Hill(float x, float y, float E, float aa, Render* rt, Network brain): _pos(x,y), _render(rt), _E(E), _ant_allocated(aa), _bounds(sf::FloatRect(_render->bounds()))
{
	assert(_bounds.contains(_pos));
	// std::cout << "CREATING HILL WITH BRAIN:" << brain << std::endl;
	// _ant_allocated = 0.01;
	for (int i = 0; i < INITIAL_ANTS; i++) {
		addAnt(brain);
	}
}

int Hill::antCount() const
{
	return _ants.size();
}

void Hill::render() const
{
	int C = antCount();
	sf::VertexArray ants(sf::Triangles, C*3*9);
	for(int i = 0; i < antCount(); i++)
	{
		if (_ants[i].render(ants, i*3))
		{
			for(int j = 0; j < 8; j++)
			{
				sf::Vector2f offset(DIRS[2*j]*_bounds.width, DIRS[2*j+1]*_bounds.height);
				ants[C*3] = ants[i*3].position+offset;
				ants[C*3+1] = ants[i*3+1].position+offset;
				ants[C*3+2] = ants[i*3+2].position+offset;
				if (_bounds.contains(ants[C*3].position) || _bounds.contains(ants[C*3+1].position) || _bounds.contains(ants[C*3+2].position))
				{
					// draw second copy of triangle if ant wraps over limits
					C++;
				}
			}
		}
	}
	// printf("%d\n", C);
	sf::Vector2f np = _pos-sf::Vector2f(HILL_SIZE, HILL_SIZE);
	ants.resize(C*3);
	_render->world()->draw(ants);
	sf::CircleShape hill(HILL_SIZE);
	hill.setPosition(np);
	hill.setFillColor(sf::Color(200,200,200,63));
	hill.setOutlineThickness(1);
	hill.setOutlineColor(sf::Color(200,200,200));
	_render->world()->draw(hill);
	for (int i = 0; i < 8; i++) {
		sf::Vector2f p = np+sf::Vector2f(_bounds.width*DIRS[i*2], _bounds.height*DIRS[i*2+1]);
		if (_bounds.contains(p-sf::Vector2f(HILL_SIZE*DIRS[i*2], HILL_SIZE*DIRS[i*2+1]))) {
			hill.setPosition(p);
			_render->world()->draw(hill);
		}
	}
}

//returns true if hill has died (no energy)
void Hill::tick() //calculate movements for all ants in hill
{
	// make sure there are ants in hill before we do anything
	if (antCount() == 0) return;

	// _render->pheromone().data[(int)_pos.y*_bounds.width+(int)_pos.x] += utils::math::polar2Cartesian(_color);

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

Hill Hill::clone(const Hill& h, const int e)  {
	assert(h.antCount() > 0);
	float aa = h._ant_allocated;
	nUtils::RANDOM_MUTATE(aa);
	// float col = h._color;
	// nUtils::RANDOM_MUTATE_THETA(col);
	// return Hill(h._pos.x+HILL_SIZE*utils::rand::urand(5,20), h._pos.y+HILL_SIZE*utils::rand::urand(5,20), e, aa, h._render, Network::random());
	// return Hill(arfmod(h._pos.x+HILL_SIZE*utils::rand::urand(2,10), h._render->bounds().width), arfmod(h._pos.y+HILL_SIZE*utils::rand::urand(2,10), h._render->bounds().height), e, aa, h._render, h._ants[utils::rand::urand(0,h._ants.size())].brain());
	return Hill(utils::rand::rand_01()*h._render->bounds().width, utils::rand::rand_01()*h._render->bounds().height, e, aa, h._render, h._ants[utils::rand::urand(0,h._ants.size())].brain());
	// h._ants[utils::rand::urand(0,h._ants.size())].brain()
}