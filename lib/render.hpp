/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-14 23:47:57
*/

#pragma once
#include <iostream>
#include "hill.hpp"

struct Render
{
public:
	Render() = default;

	Render(sf::RenderWindow* window): _window(window)
	{
	}

	void addHill(Hill h);

	void tick();

	void renderHills() const;
	std::vector<Hill> hills;
private:
	sf::RenderWindow* _window;
};