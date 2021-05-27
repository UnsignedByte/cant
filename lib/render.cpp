/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-26 23:20:26
*/

#include <iostream>
#include "render.hpp"
#include <SFML/OpenGL.hpp>

const int NEIGHBORS[] = {1, 1, 1, 0, 1, -1, 0, 1, 0, -1, -1, 1, -1, 0, -1, -1};
// 1800 = 60 seconds worth of energy

void DrawableImg::loadImg() {
	//update image with new colors
	for(int i = 0; i < _H; i++)
	{
		for (int j = 0; j < _W; j++)
		{
			_img.setPixel(j, i, utils::HS_vec_to_RGBA(data[i*_W+j]));
			// _pheromoneImg.setPixel(j, i, sf::Color(255,0,0));
			// printf("test\n");
		}
	}
}

sf::Image& DrawableImg::img() {
	return _img;
}

sf::Sprite& DrawableImg::sprite() {
	loadImg();
	_texture.loadFromImage(_img);
	_sprite.setTexture(_texture, true);
	return _sprite;
}

void Render::addHill(Hill h)
{
	h.setRender(this);
	hills.push_back(h);
}

void Render::populateRandom(int mincount, int maxcount)
{
	int count = utils::rand::urand(mincount, maxcount);

	// uses 10% of the energy to create anthills
	for (int i = 0; i < count; i++) {
		Render::addHill(Hill::randomHill(_bounds.width, _bounds.height, _TE/2/count, this));
	}
}

void Render::render()
{
	_world.clear();

	// printf("test\n");

	_world.draw(_food.sprite());
	_world.draw(_pheromone.sprite());

	// printf("test\n");

	for (int i = 0; i < hills.size(); i++)
	{
		hills[i].render();
	}
}

void Render::tick() //tick all hills and conversely all ants
{
	std::swap(_pheromone.dataOld, _pheromone.data);
	for(int i = 0; i < _bounds.height; i++)
	{
		for (int j = 0; j < _bounds.width; j++)
		{
			_pheromone.data[i*_bounds.width+j] = _pheromone.dataOld[i*_bounds.width+j]*0.995f;
			// for (int k = 0; k < 8; k++) {
			// 	_pheromone.data[i*_bounds.width+j] += _pheromone.dataOld[arimod(i+NEIGHBORS[2*k+1], _bounds.height)*_bounds.width+arimod(i+NEIGHBORS[2*k], _bounds.width)]*0.1f;
			// }
		}
	}

	// printf("%lf, %lf\n", E(), _E);

	// printf("test\n");
	int c = std::min((int) ((_TE-E()) / FOOD_CONVERSION), 5000);
	_E += c*FOOD_CONVERSION;
	// printf("%lf %lf\n", c*FOOD_CONVERSION, (_TE-E()) / FOOD_CONVERSION);
	// printf("ADDING %d food\n", c);
	for (int i = 0; i < c; i++) {
		int idx = utils::rand::urand(_bounds.top, _bounds.height)*_bounds.width + utils::rand::urand(_bounds.left, _bounds.width);
		// int idx = _bounds.width*_bounds.height/2;
		_food.data[idx] = utils::math::polar2Cartesian(2*M_PI/3, std::sqrt(utils::math::magsq(_food.data[idx]))+1);
	}
	// printf("%lf, %lf\n", E(), _E);

	// remove ants if tick() returns true, aka if ant is out of energy
	hills.erase(
		std::remove_if(hills.begin(), hills.end(),
			[](Hill& a)
			{
				a.tick();
				return a.antCount() == 0;
			}
		), hills.end());

	sf::Vector2f offset(D-A,S-W);
	updateView(offset);
}

sf::Sprite Render::getDrawn() const
{
	return sf::Sprite(_world.getTexture(), sf::IntRect(_view));
}

unsigned int Render::getGLHandle() const
{
	return _world.getTexture().getNativeHandle();
}

void Render::updateView(sf::Vector2f transposition) {
	transposition*=constants::movSpeed;
	transposition.x*=_view.width;
	transposition.y*=_view.height;
	_view.left+=transposition.x;
	_view.top+=transposition.y;
	// std::cout << view.getSize() << std::endl;
}

int Render::width() const
{
	return _bounds.width;
}

int Render::height() const
{
	return _bounds.height;
}

sf::IntRect Render::bounds() const
{
	return _bounds;
}

float Render::E() const
{
	float TE = _E;
	for(const Hill& h : hills)
	{
		TE += h.E();
	}
	return TE;
}

sf::RenderTexture* Render::world()
{
	return &_world;
}

std::vector<sf::Vector2f>& Render::pheromone()
{
	return _pheromone.data;
}

std::vector<sf::Vector2f>& Render::food()
{
	return _food.data;
}
