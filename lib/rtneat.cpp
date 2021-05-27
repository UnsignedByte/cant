/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:55
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-26 17:23:18
*/

#include <stdexcept>
#include "rtneat.hpp"
#include "utils.hpp"
#include "ant.hpp"
#include "render.hpp"

namespace nUtils {
	void RANDOM_MUTATE(float& f, bool allowFlip) {
		if (utils::rand::rand_01() < MUTATE_VALUE) {
			if (utils::rand::rand_01() < MUTATE_RESET)
			{
				f = utils::rand::norm();
			} else {
				if (allowFlip && utils::rand::rand_01() < MUTATE_FLIP)
					f *= -1;

				f *= (std::tanh(utils::rand::norm())/10+1);
			}
		}
	}

	ArgParams RANDOM_PARAMS() {
		ArgParams p;
		p.type = utils::rand::urand(0,4);
		switch(p.type) {
			case 0:
				p.offsetDir = utils::rand::norm();
				// printf("Creating constant parameter with value %f\n", p.offsetDir);
				break;
			case 1:
				// printf("Creating random parameter\n");
				break;
			case 3:
				p.matchDir = utils::rand::rand_01()*M_PI*2;
			case 2:
			case 4:
				p.offsetDir = utils::rand::rand_01()*M_PI*2-M_PI;
				p.offsetMag = utils::rand::rand_01()*MAX_SIGHT;
				// printf("Creating parameter type %d with offset (%f, %f) and match %f\n", p.type, p.offsetDir, p.offsetMag, p.matchDir);
				break;
		}
		return p;
	}

	Node RANDOM_NODE(int N) {
		Node n;

		n.bias = utils::rand::norm();

		// if (n.children.size() > 0) {
		// 	printf("SIZESIZESIZE: %ld\n", n.children.size());
		// }

		do {
			n.children.push_back(utils::rand::urand(0,N));
			n.weights.push_back(utils::rand::norm());
		} while (utils::rand::rand_01() < INITIAL_CHILD_CHANCE);
		return n;
	}
}

float Network::parseArg(int i, Ant* a) const
{
	sf::Vector2f offsetPos = a->getPos()+utils::math::polar2Cartesian(a->getAngle().getAngle()+_argparams[i].offsetDir, _argparams[i].offsetMag);
	int W = a->render()->bounds().width;
	int H = a->render()->bounds().height;
	int idx = arimod((int)offsetPos.y, H) * W + arimod((int) offsetPos.x, W);
	switch(_argparams[i].type) {
		case 0: // return a defined constant
			return _argparams[i].offsetDir;
		case 1: // Return a random number from (-1,1)
			return utils::rand::rand_01()*2-1;
		case 2: // PHERMONE INTENSITY (squared) at offset
		case 3: // PHERMONE Activation by color
		{
			sf::IntRect bounds = a->render()->bounds();

			//Color vector at point
			sf::Vector2f col = a->render()->pheromone()[idx];
			if (_argparams[i].type == 1)
				return utils::math::magsq(col);
			else {
				// circular radian difference between two angles, normalized to (-1,1).
				float d = arfmod(d-_argparams[i].matchDir, M_PI*2);
				d = 1-std::min(d, (float)M_PI*2-d)/M_PI;
				// square to emphasize values near 1
				return d*d;
			}
		}
		case 4: // FOOD amount at offset
		{
			//return magnitude squared of color vector at point
			return utils::math::magsq(a->render()->food()[idx]);
		}
	}
	return 0;
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
			if (_nodes[i].children[j] > _nodes.size()) {
				std::cout << *this << std::endl;
				std::cout << _nodes[i].children << std::endl;
				printf("%d, %d, %d\n", _nodes[i].children[j], i, j);
				throw std::runtime_error("Fucked");
			}
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

void Network::prune() {

	bool visited[N()] = {0};
	// TRUE if node is not a dead end
	bool dead[N()] = {0};

	for (int i = 0; i < _inputs.size(); i++) {
		bool valid = 0;
		for (int j = 0; j < _inputs[i].children.size(); j++) {
			bool k = _prune_prop(_inputs[i].children[j], visited, dead);
			valid |= k;

			// printf("@ Input %d: Testing %d\n", i, _inputs[i].children[j]);
			// leads to a dead end, remove this weight
			if (!k) {
				// printf("@Input %d:Node %d is dead, pruning weight\n", i, _inputs[i].children[j]);
				_inputs[i].children.erase(_inputs[i].children.begin()+j);
				_inputs[i].weights.erase(_inputs[i].weights.begin()+j);
				j--;
			}
		}

		// prune input
		if (!valid) {
			_inputs.erase(_inputs.begin()+i);
			_argparams.erase(_argparams.begin()+i);
			i--;
		}
	}

	// FIX child propogation IDs now that nodes will be removed
	// rem[i] is how much node i needs to be shifted
	int rem[N()];
	rem[0] = !(visited[0] && dead[0]);
	for (int i = 1; i < N(); i++) {
		rem[i] = rem[i-1]+!(visited[i] && dead[i]);
		// printf("REMOVE IND %d = %d, visited %d, dead %d\n", i, rem[i], visited[i], dead[i]);
	}

	int ind = 0;
	_nodes.erase(
		std::remove_if(_nodes.begin()+_ocount, _nodes.end(),
			[&ind, &dead, &visited](Node& n) {
				// A node must be VISITED and not a dead end to survive (dead[i] is dead if FALSE)
				// printf("Node %d returns %d\n", ind, !(visited[ind] && dead[ind]));
				return !(visited[ind++] && dead[ind-1]);
			}
		), _nodes.end());

	// prune from states as well
	ind = 0;
	_states.erase(
		std::remove_if(_states.begin()+_ocount, _states.end(),
			[&ind, &dead, &visited](float& n) {
				// A node must be VISITED and not a dead end to survive (dead[i] is dead if FALSE)
				return !(visited[ind++] && dead[ind-1]);
			}
		), _states.end());

	// fix inputs
	for(int i = 0; i < _inputs.size(); i++) {
		for (int j = 0; j < _inputs[i].children.size(); j++) {
			// skip if it leads to an output node
			if (_inputs[i].children[j] < _ocount) continue;

			// printf("Shifting input node from %d to %d by %d\n", i, _inputs[i].children[j], rem[_inputs[i].children[j]-_ocount]);
			_inputs[i].children[j] -= rem[_inputs[i].children[j]-_ocount];
		}
	}

	// fix nodes
	for(int i = _ocount; i < _nodes.size(); i++) {
		for (int j = 0; j < _nodes[i].children.size(); j++) {
			// skip if it leads to an output node
			if (_nodes[i].children[j] < _ocount) continue;

			// printf("Shifting input node from %d to %d by %d\n", i, _nodes[i].children[j], rem[_nodes[i].children[j]-_ocount]);
			_nodes[i].children[j] -= rem[_nodes[i].children[j]-_ocount];
		}
	}
}

// return FALSE if node is a dead end/leads to only dead ends
bool Network::_prune_prop(int i, bool* visited, bool* dead) {
	// if (i < 0 || i > _nodes.size())
	// 	std::cout << *this << std::endl;
	// I is an output node OR leads to a loop
	if (i < _ocount) return 1;

	// printf("@ node %d: Visited %d, dead %d\n", i, visited[i-_ocount], dead[i-_ocount]);
	// if the node has been visited, return its calculated value
	if (visited[i-_ocount]) return dead[i-_ocount];

	//set to visited
	visited[i-_ocount] = 1;

	//should initially already be 0
	dead[i-_ocount] = 1;

	bool d;

	for (int j = 0; j < _nodes[i].children.size(); j++) {
		// return TRUE if node is self-referencing, or if it is prunable
		// printf("@ node %d: Testing %d\n", i, _nodes[i].children[j]);
		bool k = _nodes[i].children[j] == i || _prune_prop(_nodes[i].children[j], visited, dead);
		d |= k;
		// printf("%d, %d\n", _nodes[i].children[j] == i, k);

		// leads to a dead end, remove this weight
		if (!k) {
			// printf("@ node %d: Node %d is dead, pruning weight\n", i, _nodes[i].children[j]);
			_nodes[i].children.erase(_nodes[i].children.begin()+j);
			_nodes[i].weights.erase(_nodes[i].weights.begin()+j);
			j--;
		}
	}

	dead[i-_ocount] = d;

	return d;
}