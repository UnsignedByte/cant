/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 17:47:56
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

		// create empty food image
		_food.resize(WIDTH*HEIGHT);
		_foodImg.create(WIDTH, HEIGHT);
		_foodTexture.create(WIDTH, HEIGHT);

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
	std::vector<sf::Vector2f>& food();

	sf::IntRect bounds() const;

	sf::Sprite getDrawn() const;

	sf::RenderTexture* world();

	std::vector<Hill> hills;
private:
	sf::FloatRect _view;
	sf::IntRect _bounds;
	sf::Image _foodImg;
	sf::Texture _foodTexture;
	sf::Sprite _foodSprite;
	std::vector<sf::Vector2f> _food;
	sf::RenderTexture _world;
	int _E;
};