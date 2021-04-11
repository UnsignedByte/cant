#include <SFML/Graphics.hpp>
#include "ant.hpp"
#include <cstring>

sf::CircleShape* Ant::render()
{

	// auto self = std::make_unique<sf::CircleShape>(10.f, 3);
	auto self = new sf::CircleShape(10.f, 3);

	self->setPosition(pos);
	self->setFillColor(sf::Color(urand(0,256), urand(0,256), urand(0,256)));

	return self;
}