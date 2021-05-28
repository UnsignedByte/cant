/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:27
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 22:22:27
*/

#pragma once
#include <iostream>
#include "hill.hpp"
#include <stdexcept>
#include <SFML/OpenGL.hpp>

const float FOOD_CONVERSION = 1.f;
const float FOOD_OPACITY = 0.3f;
const float PHEROMONE_OPACITY = 5.f;

const int NEIGHBORS[] = {1, 1, 1, 0, 1, -1, 0, 1, 0, -1, -1, 1, -1, 0, -1, -1};
const float DECAY_RATE = 0.01f;
const float FOOD_DECAY_RATE = 0.001f;
const float DEACTIVATE_MAG = 0.001;
const float HILL_BIRTH_CHANCE = 1.f/900.f;
const float RANDOM_HILL_CHANCE = 0.01f;
// chance of a given pixel to spread food
const float SPREAD_FOOD_CHANCE = 0.01;

struct DrawableImg {
	DrawableImg() = default;

	DrawableImg(int WIDTH, int HEIGHT, float opacity = 1): _W(WIDTH), _H(HEIGHT), _opacity(opacity) {
		_img.create(_W, _H);
		_texture.create(_W, _H);
		data.resize(WIDTH*HEIGHT);
		dataOld.resize(WIDTH*HEIGHT);
		active.resize(WIDTH*HEIGHT,0);
		activeOld.resize(WIDTH*HEIGHT,0);
	}

	void loadImg();

	sf::Image& img();
	sf::Sprite& sprite();

	std::vector<sf::Vector2f> data;
	std::vector<sf::Vector2f> dataOld;
	std::vector<bool> active;
	std::vector<bool> activeOld;
private:
	int _W, _H;
	float _opacity;
	sf::Sprite _sprite;
	sf::Texture _texture;
	sf::Image _img;
};

struct Render
{
public:
	bool W=0, A=0, S=0, D=0;
	Render() = default;

	Render(int WIDTH, int HEIGHT, float E): _view(0, 0, WIDTH, HEIGHT), _bounds(0, 0, WIDTH, HEIGHT), _TE(E)
	{
		if (!_world.create(WIDTH, HEIGHT))
		{
			throw(std::runtime_error("Could not create RenderTexture"));
		}

		_world.setRepeated(true);

		// create empty pheromone image
		_pheromone = DrawableImg(WIDTH, HEIGHT, PHEROMONE_OPACITY);
		_food = DrawableImg(WIDTH, HEIGHT, FOOD_OPACITY*(WIDTH*HEIGHT)/_TE);

		// sf::Texture::bind(&_world.getTexture());

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void addHill(Hill);

	void populateRandom(int, int);

	void tick();

	void render();

	void updateView(sf::Vector2f);

	unsigned int getGLHandle() const;

	int width() const;
	int height() const;

	float E() const;
	DrawableImg& pheromone();
	DrawableImg& food();

	sf::IntRect bounds() const;

	sf::Sprite getDrawn() const;

	sf::RenderTexture* world();

	std::vector<Hill> hills;
	float _E = 0;
private:
	sf::FloatRect _view;
	const sf::IntRect _bounds;
	DrawableImg _pheromone;
	DrawableImg _food;
	sf::RenderTexture _world;
	float _hill_allocated = 0;
	const float _TE;
};