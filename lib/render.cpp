/*
* @Author: UnsignedByte
* @Date:   2021-04-11 16:32:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-28 01:06:03
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
			// DEBUG VIEW: shows whether a pixel is active
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
	hills.push_back(h);
}

void Render::populateRandom(int mincount, int maxcount)
{
	int count = utils::rand::urand(mincount, maxcount);
	_hill_allocated = INITIAL_ANTS*MAX_STOMACH_SIZE/2;

	// uses some of the energy to create anthills
	for (int i = 0; i < count; i++) {
		Render::addHill(Hill::randomHill(_bounds.width, _bounds.height, std::min(_hill_allocated, _TE/2/count), this));
	}

	// fill world using half the remaining energy
	std::fill(_food.data.begin(), _food.data.end(), utils::math::polar2Cartesian(2*M_PI/3, (_TE-std::min(_hill_allocated, _TE/2/count)*count)/2/_bounds.width/_bounds.height/FOOD_CONVERSION));
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

void Render::tick() //tick all hills and then all ants
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
			_food.data[idx]*=(1-FOOD_DECAY_RATE);
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


	if (utils::rand::rand_01() < HILL_BIRTH_CHANCE) {
		if (hills.size() > 0 && utils::rand::rand_01() > RANDOM_HILL_CHANCE) {
			addHill(Hill::clone(hills[utils::rand::urand(0,hills.size())], _hill_allocated));
		} else {
			addHill(Hill::randomHill(_bounds.width, _bounds.height, _hill_allocated, this));
		}	
	}

	float remainingE = std::max(0.f, _TE-E()-_hill_allocated);
	// printf("%f, %f, %f, %f\n", E(), _E, _TE, remainingE);
	// printf("test\n");
	int c = utils::rand::urand(0,(int) std::min(remainingE , 100.f)/FOOD_CONVERSION);
	// _E += c*FOOD_CONVERSION;
	// printf("%lf %lf\n", c*FOOD_CONVERSION, (_TE-E()) / FOOD_CONVERSION);
	// printf("ADDING %d food\n", c);
	for (int i = 0; i < c; i++) {
		int idx = utils::rand::urand(0, _bounds.height)*_bounds.width + utils::rand::urand(0, _bounds.width);
		// int idx = _bounds.width*_bounds.height/2;
		_food.data[idx] = utils::math::polar2Cartesian(2*M_PI/3, utils::math::mag(_food.data[idx])+1);
	}

	// FOOD SPREADING

	c = (int) _bounds.width*_bounds.height*SPREAD_FOOD_CHANCE;
	for (int i = 0; i < c; i++) {
		sf::Vector2f idx(utils::rand::urand(0, _bounds.height), utils::rand::urand(0, _bounds.width));
		sf::Vector2f offset = arfmod(idx+sf::Vector2f(utils::rand::urand(-3,4), utils::rand::urand(-3,4)), sf::Vector2f(_bounds.width, _bounds.height));
		int ii = idx.y*_bounds.width+idx.x;
		int oi = offset.y*_bounds.width+offset.x;
		// int idx = _bounds.width*_bounds.height/2;
		_food.data[oi] = utils::math::polar2Cartesian(2*M_PI/3, std::min(MAX_STOMACH_SIZE/FOOD_CONVERSION/16.f, utils::math::mag(_food.data[oi])+std::min(remainingE, 0.5f*utils::math::mag(_food.data[ii]))));
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
