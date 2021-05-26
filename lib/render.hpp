/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 23:08:08
*/

#pragma once
#include <iostream>
#include "hill.hpp"
#include <stdexcept>
#include <SFML/OpenGL.hpp>

struct DrawableImg {
	DrawableImg() = default;

	DrawableImg(int WIDTH, int HEIGHT): _W(WIDTH), _H(HEIGHT) {
		_img.create(_W, _H);
		_texture.create(_W, _H);
		data.resize(WIDTH*HEIGHT);
		dataOld.resize(WIDTH*HEIGHT);
	}

	void loadImg();

	sf::Image& img();
	sf::Sprite& sprite();

	std::vector<sf::Vector2f> data;
	std::vector<sf::Vector2f> dataOld;
private:
	int _W, _H;
	sf::Sprite _sprite;
	sf::Texture _texture;
	sf::Image _img;
};

struct Render
{
public:
	bool W=0, A=0, S=0, D=0;
	Render() = default;

	Render(int WIDTH, int HEIGHT, int E): _view(0, 0, WIDTH, HEIGHT), _bounds(0, 0, WIDTH, HEIGHT), _TE(E)
	{
		if (!_world.create(WIDTH, HEIGHT))
		{
			throw(std::runtime_error("Could not create RenderTexture"));
		}

		_world.setRepeated(true);

		// create empty pheromone image
		_pheromone = DrawableImg(WIDTH, HEIGHT);
		_food = DrawableImg(WIDTH, HEIGHT);

		// sf::Texture::bind(&_world.getTexture());

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void addHill(Hill);

	void populateRandom(int, int, int, int);

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
	const sf::IntRect _bounds;
	DrawableImg _pheromone;
	DrawableImg _food;
	sf::RenderTexture _world;
	int _E;
	const int _TE;
};