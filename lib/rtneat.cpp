/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:55
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 22:07:52
*/

#include <stdexcept>
#include "rtneat.hpp"
#include "utils.hpp"
#include "ant.hpp"
#include "render.hpp"

namespace nUtils {
	void RANDOM_MUTATE(float& f) {
		if (utils::rand::rand_01() < MUTATE_VALUE) {
			if (utils::rand::rand_01() < MUTATE_RESET)
				f = utils::rand::norm();

			f *= (std::tanh(utils::rand::norm())/10+1);
		}
	}

	ArgParams RANDOM_PARAMS() {
		ArgParams p;
		p.type = utils::rand::urand(0,4);
		switch(p.type) {
			case 0:
				p.offsetDir = utils::rand::norm();
				printf("Creating constant parameter with value %f\n", p.offsetDir);
				break;
			case 1:
				printf("Creating random parameter\n");
				break;
			case 3:
				p.matchDir = utils::rand::rand_01()*M_PI*2;
			case 2:
				p.offsetDir = utils::rand::rand_01()*M_PI*2-M_PI;
				p.offsetMag = utils::rand::norm();
				printf("Creating parameter type %d with offset (%f, %f) and match %f\n", p.type, p.offsetDir, p.offsetMag, p.matchDir);
				break;
		}
		return p;
	}

	Node RANDOM_NODE(int N) {
		Node n;

		n.bias = utils::rand::norm();

		if (n.children.size() > 0) {
			printf("SIZESIZESIZE: %ld\n", n.children.size());
		}

		do {
			n.children.push_back(utils::rand::urand(0,N));
			n.weights.push_back(utils::rand::norm());
		} while (utils::rand::rand_01() < INITIAL_CHILD_CHANCE);
		return n;
	}
}

void Network::propogate()
{
	for(int i = _ocount; i < _nodes.size(); i++)
	{
		_states[i] = std::tanh(_nodes[i].state);
	}

	for(int i = 0; i < _nodes.size(); i++) {
		//reset state to accept new
		_nodes[i].state = 0;
	}

	for(int i = 0; i < _inputs.size(); i++)
	{
		for (int j = 0; j < _inputs[i].children.size(); j++)
		{
			// printf("%d, %d, %d\n", _inputs[i].children[j], i, j);
			_nodes[_inputs[i].children[j]].state += _inputs[i].state*_inputs[i].weights[j] + _inputs[i].bias;
			// printf("Propogating input to child %d with state %f\n", C,  _nodes[C].state);
		}
	}

	for(int i = _ocount; i < _nodes.size(); i++)
	{
		for (int j = 0; j < _nodes[i].children.size(); j++)
		{
			// if (_nodes[i].children[j] > _nodes.size()) {
			// 	std::cout << *this << std::endl;
			// 	std::cout << _nodes[i].children << std::endl;
			// 	printf("%d, %d, %d\n", _nodes[i].children[j], i, j);
			// 	throw std::runtime_error("Fucked");
			// }
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
	return _nodes[i].state;
}

float Network::parseArg(int i, Ant* a) const
{
	switch(_argparams[i].type) {
		case 0: // return a defined constant
			return _argparams[i].offsetDir;
		case 1: // Return a random number from (-1,1)
			return utils::rand::rand_01()*2-1;
		case 2: // PHERMONE INTENSITY (squared) at offset
		case 3: // PHERMONE Activation by color
			sf::Vector2f offsetPos = a->getPos()+utils::math::polar2Cartesian(a->getAngle().getAngle()+_argparams[i].offsetDir, _argparams[i].offsetMag);
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

int Network::N() const
{
	return _nodes.size()-_ocount;
}

std::ostream& operator<<(std::ostream &os, const Network& n) {
	os << "\nNetwork with " << n.N() << " nodes with " << n._inputs.size() << " inputs and " << n._ocount << " outputs\n";

	for(int i = 0; i < n._inputs.size(); i++) {
		os << "INPUT NODE " << i << " has bias " << n._inputs[i].bias << " and the following children:\n";
		for (int j = 0; j < n._inputs[i].children.size(); j++) {
			os << "\tCHILD " << n._inputs[i].children[j] << " with weight " << n._inputs[i].weights[j] << "\n";
		}
	}

	for(int i = n._ocount; i < n._nodes.size(); i++) {
		os << "NODE " << i << " has bias " << n._nodes[i].bias << " and the following children:\n";
		for (int j = 0; j < n._nodes[i].children.size(); j++) {
			os << "\tCHILD " << n._nodes[i].children[j] << " with weight " << n._nodes[i].weights[j] << "\n";
		}
	}

	return os;
}