/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-15 12:25:02
*/

#pragma once
#include <iostream>
#include "hill.hpp"

struct Render
{
public:
	Render() = default;

	Render(sf::RenderTexture* world): _world(world)
	{
	}

	void addHill(Hill h);

	void tick();

	void renderHills() const;

	sf::RenderTexture* getTex() const;

	std::vector<Hill> hills;
private:
	sf::RenderTexture* _world;
};