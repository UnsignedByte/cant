/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:55
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 08:25:09
*/

#include "rtneat.hpp"
#include "utils.hpp"
#include "ant.hpp"
#include "render.hpp"

void Network::propogate()
{
	for(int i = 0; i < _N; i++)
	{
		_states[i] = utils::math::sigmoid(_nodes[i].state);
	}

	for(int i = 0; i < _nodes.size(); i++) {
		//reset state to accept new
		_nodes[i].state = 0;
	}

	for(int i = 0; i < _inputs.size(); i++)
	{
		for (int j = 0; j < _inputs[i].children.size(); j++)
		{
			_nodes[_inputs[i].children[j]].state += _inputs[i].state*_inputs[i].weights[j] + _inputs[i].bias;
			// printf("Propogating input to child %d with state %f\n", C,  _nodes[C].state);
		}
	}

	for(int i = 0; i < _N; i++)
	{
		for (int j = 0; j < _nodes[i].children.size(); j++)
		{
			_nodes[_nodes[i].children[j]].state += _states[i]*_nodes[i].weights[j] + _nodes[i].bias;
			// printf("Propogating to child %d with state %f\n", C, _states[i] + _nodes[C].state);
		}
	}

	// printf("%f\n", _inputs[0].state);
	// printf("%f\n", _nodes[0].state);
	// printf("%f\n", _nodes[1].state);
}

void Network::tick(Ant* a)
{
	for(int i = 0; i < _inputs.size(); i++)
	{
		_inputs[i].state = parseArg(i, a);
	}

	propogate();
}

float Network::output(int i) const
{
	return _nodes[_N+i].state;
}

float Network::parseArg(int i, Ant* a) const
{
	switch(_argparams[i].type) {
		case 0: // Return a random number
			return utils::rand::rand_01();
		case 1: // PHERMONE INTENSITY (squared) at offset
		case 2: // PHERMONE Activation by color
			sf::Vector2f offsetPos = a->getPos()+utils::math::Angle(a->getAngle()+_argparams[i].offsetDir).getVec();
			sf::IntRect bounds = a->render()->bounds();

			//Color vector at point
			sf::Vector2f col = a->render()->pheromone()[arimod((int)offsetPos.y, bounds.height) * bounds.width + arimod((int) offsetPos.x, bounds.width)];
			if (_argparams[i].type == 1)
				return col.x*col.x+col.y*col.y;
			else {
				// circular radian difference between two angles, normalized to (-1,1).
				float d = arfmod(d-_argparams[i].matchDir, M_PI*2);
				d = 1-std::min(d, (float)M_PI*2-d)/M_PI;
				// square to emphasize values near 1
				return d*d;
			}
	}
	return 0;
}