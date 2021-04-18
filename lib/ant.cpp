/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-15 10:27:58
*/
#include <SFML/Graphics.hpp>
#include "ant.hpp"
#include <cstring>
#include "utils.hpp"
#include <cmath>
#define ANT_SIZE 3.f;

void Ant::move()
{
	_dir+=(utils::rand::rand_01()-0.5f)/2.f;
	_pos+=_dir.getVec();
}

void Ant::render(sf::VertexArray& arr, int i) const
{
	arr[i].position = _pos;
	arr[i+1].position = _pos+(_dir+5.f/6*M_PI).getVec()*ANT_SIZE-_dir.getVec()*ANT_SIZE;
	arr[i+2].position = _pos+(_dir-5.f/6*M_PI).getVec()*ANT_SIZE-_dir.getVec()*ANT_SIZE;

	// set color of vertices
	arr[i].color = sf::Color(255, 255, 255, 255);
	arr[i+1].color = sf::Color(255, 255, 255, 255);
	arr[i+2].color = sf::Color(255, 255, 255, 255);

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