/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-11 12:31:46
*/
#include <SFML/Graphics.hpp>
#include "ant.hpp"
#include <cstring>
#include "utils.hpp"

void Ant::move()
{
	_dir+=(utils::rand::rand_01()-0.5f)/2.f;
	_pos+=_dir.getVec();
}

sf::CircleShape* Ant::render()
{

	// auto self = std::make_unique<sf::CircleShape>(10.f, 3);
	auto self = new sf::CircleShape(10.f, 3);

	self->setPosition(_pos);
	self->setFillColor(sf::Color(utils::rand::urand(0,256), utils::rand::urand(0,256), utils::rand::urand(0,256)));

	return self;
}

sf::Vector2f Ant::getPos() const
{
	return _pos;
}

utils::math::Angle Ant::getAngle() const
{
	return _dir;
}