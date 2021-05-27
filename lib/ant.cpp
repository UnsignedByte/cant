/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-26 18:52:07
*/
#include <SFML/Graphics.hpp>
#include "ant.hpp"
#include <cstring>
#include "utils.hpp"
#include <cmath>
#include "render.hpp"
#include "hill.hpp"

const float ANT_SIZE = 1.5f;

// const int DOT[] = {0, 0, 0, 1, 0, -1, -1, 0, 1, 0};

// returns TRUE if ant has died
void Ant::tick()
{
	// _dir+=(utils::rand::rand_01()-0.5f)/2.f;
	_brain.tick(this);

	int pidx = (int)_pos.y * _render->bounds().width + (int)_pos.x; // current screen position

	//current amount of food
	float currFood = std::sqrt(utils::math::magsq(_render->food()[pidx]));
	// eat food at tile
	float eatVal = std::min(currFood*FOOD_CONVERSION, _stomach_size-_E);
	// if (eatVal > 0) {
	// 	std::cout << _render->food()[pidx] << std::endl;
	// 	printf("%d: %f %f\n", pidx, utils::math::magsq(_render->food()[pidx]), eatVal);
	// }

	_E += eatVal;
	// remove this food from the screen
	_render->food()[pidx] = utils::math::polar2Cartesian(2*M_PI/3, currFood-eatVal/FOOD_CONVERSION);
	// remove this food from global energy use
	_render->_E -= eatVal;

	// drop pheromone at tile
	_render->pheromone()[pidx] += utils::math::polar2Cartesian(_brain.output(1))*std::tanh(_brain.output(2));

	// move
	_dir+=std::tanh(_brain.output(0))*M_PI/8;
	_pos+=_dir.getVec();

	// std::cout << _render << std::endl;

	_pos = arfmod(_pos, _render->world()->getSize());

	// _render->pheromone()[(int)_pos.y * _render->bounds().width + (int)_pos.x] += utils::math::polar2Cartesian(0);
	// for(int i = 0; i < sizeof(DOT)/sizeof(int)/2; i++){
	// 	_render->pheromone()[arimod((int)_pos.y+DOT[i*2+1], _render->bounds().height) * _render->bounds().width + arimod((int) _pos.x + DOT[i*2], _render->bounds().width)] += utils::math::Angle(_brain.output(1)).getVec();
	// }
	
	// sf::Color test = utils::HSVec2RGB(_dir.getVec());
	// printf("%d, %d, %d\n", test.r, test.g, test.b);
	_E--;
}

bool Ant::render(sf::VertexArray& arr, int i) const
{
	arr[i].position = _pos;
	arr[i+1].position = _pos+(_dir+5.f/6*M_PI).getVec()*ANT_SIZE-_dir.getVec()*ANT_SIZE;
	arr[i+2].position = _pos+(_dir-5.f/6*M_PI).getVec()*ANT_SIZE-_dir.getVec()*ANT_SIZE;

	// set color of vertices
	arr[i].color = sf::Color(255, 255, 255, 255);
	arr[i+1].color = sf::Color(255, 255, 255, 255);
	arr[i+2].color = sf::Color(255, 255, 255, 255);

	//if any of the ant's vertices are out of bounds

	sf::FloatRect bounds = sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f(_render->world()->getSize()));

	if (!(bounds.contains(arr[i].position) && bounds.contains(arr[i+1].position) && bounds.contains(arr[i+2].position)))
	{
		return 1;
	}

	return 0;

	// auto self = std::make_unique<sf::CircleShape>(10.f, 3);
	// auto self = new sf::CircleShape(10.f, 3);

	// self->setPosition(_pos);
	// self->setFillColor(sf::Color(utils::rand::urand(0,256), utils::rand::urand(0,256), utils::rand::urand(0,256)));

	// return self;
}

sf::Vector2f Ant::getPos() const
{
	return _pos;
}

utils::math::Angle Ant::getAngle() const
{
	return _dir;
}

float Ant::E() const
{
	return _E;
}

float Ant::setE(float available)
{
	float d = std::min(available,_stomach_equil-_E);
	_E += d;
	return d;
}

Render* Ant::render()
{
	return _render;
}