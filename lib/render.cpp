/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 15:03:49
*/

#include <iostream>
#include "render.hpp"
#include <SFML/OpenGL.hpp>
// 1800 = 60 seconds worth of energy

void DrawableImg::loadImg() {
	//update image with new colors
	for(int i = 0; i < _H; i++)
	{
		for (int j = 0; j < _W; j++)
		{
			// _img.setPixel(j, i, sf::Color(255,255,255,active[i*_W+j]*255));
			_img.setPixel(j, i, utils::HS_vec_to_RGBA(data[i*_W+j], _opacity));
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

	// uses some of the energy to create anthills
	for (int i = 0; i < count; i++) {
		Render::addHill(Hill::randomHill(_bounds.width, _bounds.height, _TE/2/count, this));
	}
}

void Render::render()
{
	sf::Vector2f offset(D-A,S-W);
	updateView(offset);

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

	// int ACTIVE_COUNT = 0;
	// recalculate global E
	_E = 0;
	_pheromone.data.swap(_pheromone.dataOld);
	_pheromone.active.swap(_pheromone.activeOld);
	for(int i = 0; i < _bounds.height; i++)
	{
		for (int j = 0; j < _bounds.width; j++)
		{
			int idx = i*_bounds.width+j;
			_E += utils::math::mag(_food.data[idx]);

			_pheromone.data[idx] = sf::Vector2f(0,0);
			_pheromone.active[idx] = 0;
			if (_pheromone.activeOld[idx])
			{
				if (utils::math::magsq(_pheromone.dataOld[idx]) > DEACTIVATE_MAG*DEACTIVATE_MAG) {
					// ACTIVE_COUNT++;
					_pheromone.active[idx] = 1;
					_pheromone.data[idx] = _pheromone.dataOld[idx]*(1.f-DECAY_RATE);
					
					sf::Vector2f mn = _pheromone.dataOld[idx]*DECAY_RATE/8.f;

					// loop through neighbours and spread pheromone if above cutoff
					if (utils::math::magsq(mn) > DEACTIVATE_MAG*DEACTIVATE_MAG) {
						for (int k = 0; k < 8; k++) {
							int nidx = arimod(i+NEIGHBORS[2*k+1], _bounds.height)*_bounds.width+arimod(j+NEIGHBORS[2*k], _bounds.width);
							_pheromone.data[nidx] += mn;
							_pheromone.active[nidx] = 1;
						}
					}
				}
			}
		}
	}

	// printf("%d out of %d active; deactivated %d\n", ACTIVE_COUNT, _bounds.width*_bounds.height, DEACTIVATED_COUNT);

	// printf("%lf, %lf\n", E(), _E);

	// printf("test\n");
	int c = utils::rand::urand(0,std::min((int) ((_TE-E()) / FOOD_CONVERSION), 10000));
	// _E += c*FOOD_CONVERSION;
	// printf("%lf %lf\n", c*FOOD_CONVERSION, (_TE-E()) / FOOD_CONVERSION);
	// printf("ADDING %d food\n", c);
	for (int i = 0; i < c; i++) {
		int idx = utils::rand::urand(_bounds.top, _bounds.height)*_bounds.width + utils::rand::urand(_bounds.left, _bounds.width);
		// int idx = _bounds.width*_bounds.height/2;
		_food.data[idx] = utils::math::polar2Cartesian(2*M_PI/3, utils::math::mag(_food.data[idx])+1);
	}
	// printf("%lf, %lf\n", E(), _E);

	// remove hill if out of ants
	hills.erase(
		std::remove_if(hills.begin(), hills.end(),
			[](Hill& a)
			{
				a.tick();
				return a.antCount() == 0;
			}
		), hills.end());
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

DrawableImg& Render::pheromone()
{
	return _pheromone;
}

DrawableImg& Render::food()
{
	return _food;
}
