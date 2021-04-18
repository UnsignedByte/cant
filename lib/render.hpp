/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-16 23:27:30
*/

#pragma once
#include <iostream>
#include "hill.hpp"
#include <stdexcept>

struct Render
{
public:
	bool W=0, A=0, S=0, D=0;
	Render() = default;

	Render(int WIDTH, int HEIGHT): WIDTH(WIDTH), HEIGHT(HEIGHT)
	{
		if (!_world.create(WIDTH, HEIGHT))
		{
			throw(std::runtime_error("Could not create RenderTexture"));
		}
	}

	void addHill(Hill);

	void tick();

	void renderHills();

	void updateView(sf::Vector2f&);

	sf::Sprite getDrawn() const;

	std::vector<Hill> hills;
private:
	sf::RenderTexture _world;
	int WIDTH;
	int HEIGHT;
};