/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 18:14:38
*/

#pragma once
#include <iostream>
#include "hill.hpp"
#include <stdexcept>
#include <SFML/OpenGL.hpp>

struct Render
{
public:
	bool W=0, A=0, S=0, D=0;
	Render() = default;

	Render(int WIDTH, int HEIGHT): _view(0, 0, WIDTH, HEIGHT), _bounds(0, 0, WIDTH, HEIGHT)
	{
		if (!_world.create(WIDTH, HEIGHT))
		{
			throw(std::runtime_error("Could not create RenderTexture"));
		}

		_world.setRepeated(true);

		// create empty pheromone image
		_pheromone.resize(WIDTH*HEIGHT);
		_pheromoneImg.create(WIDTH, HEIGHT);
		_pheromoneTexture.create(WIDTH, HEIGHT);

		// sf::Texture::bind(&_world.getTexture());

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void addHill(Hill);

	void tick();

	void render();

	void updateView(sf::Vector2f);

	unsigned int getGLHandle() const;

	int width() const;
	int height() const;

	int E() const;
	std::vector<sf::Vector2f>& pheromone();

	sf::IntRect bounds() const;

	sf::Sprite getDrawn() const;

	sf::RenderTexture* world();

	std::vector<Hill> hills;
private:
	sf::FloatRect _view;
	sf::IntRect _bounds;
	sf::Image _pheromoneImg;
	sf::Texture _pheromoneTexture;
	sf::Sprite _pheromoneSprite;
	std::vector<sf::Vector2f> _pheromone;
	sf::RenderTexture _world;
	int _E;
};