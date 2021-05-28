/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:55
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-28 01:05:59
*/

#include <stdexcept>
#include "rtneat.hpp"
#include "utils.hpp"
#include "ant.hpp"
#include "render.hpp"

namespace nUtils {
	void RANDOM_MUTATE(float& f, bool allowFlip, float max) {
		if (utils::rand::rand_01() < MUTATE_VALUE) {
			if (utils::rand::rand_01() < MUTATE_RESET)
			{
				f = utils::rand::norm();
			} else {
				if (allowFlip && utils::rand::rand_01() < MUTATE_FLIP)
					f *= -1;

				f *= (std::tanh(utils::rand::norm())/10+1);
			}
			f = std::min(f, max);
		}
	}

	void RANDOM_MUTATE_THETA(float& f) {
		if (utils::rand::rand_01() < MUTATE_VALUE) {
			if (utils::rand::rand_01() < MUTATE_RESET)
			{
				f = utils::rand::rand_01()*M_PI*2;
			} else {
				f += (utils::rand::rand_01()-0.5)*M_PI*2.f/8.f;
				f = arfmod(f, 2*M_PI);
			}
		}
	}

	void RANDOM_MUTATE_01(float& f) {
		if (utils::rand::rand_01() < MUTATE_VALUE) {
			if (utils::rand::rand_01() < MUTATE_RESET)
			{
				f = utils::rand::rand_01();
			} else {
				f += (utils::rand::rand_01()-0.5)*0.2;
				f = std::max(std::min(f, 1.f),0.f);
			}
		}
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

	void MUTATE_ARG_PARAMS(ArgParams& a) {
		RANDOM_MUTATE(a.UNBOUND);
		RANDOM_MUTATE_THETA(a.DIR1);
		RANDOM_MUTATE_THETA(a.DIR2);
		RANDOM_MUTATE_01(a.R01);
	}

	ArgParams::ArgParams() {
		type = utils::rand::urand(0,5);
		UNBOUND = utils::rand::norm();
		DIR1 = utils::rand::rand_theta(); // MATCH COLOR
		DIR2 = utils::rand::rand_theta(); // VIEW RANGE
		R01 = utils::rand::rand_01();
	}

	std::vector<int> POINTS_ON_ARC(Ant* a, const float tmin, const float tmax, const float dist, const float sparsity) {
		std::vector<int> out;

		sf::Vector2f p;
		if (dist == 0) {
			p = a->getPos();
			out.push_back((int)p.y * a->render()->bounds().width+(int)p.x);
			return out;
		}

		for(int i = 0; i < dist*(tmax-tmin)*sparsity*M_PI; i++) {
			p = arfmod(a->getPos()+utils::math::polar2Cartesian(tmin+i/sparsity/dist/M_PI, dist), sf::Vector2f(a->render()->world()->getSize()));
			// std::cout << p << std::endl;
			out.push_back((int)p.y * a->render()->bounds().width+(int)p.x);
		}
		return out;
	}
}

float Network::parseArg(const nUtils::ArgParams& p, Ant* a) const
{
	// std::cout << a->getAngle() << std::endl;
	std::vector<int> v = nUtils::POINTS_ON_ARC(a, a->getAngle()-p.DIR1*MAX_SIGHT_RANGE, a->getAngle()+p.DIR1*MAX_SIGHT_RANGE, p.R01*MAX_SIGHT);
	// std::cout << v << std::endl;
	// sf::Vector2f offsetPos = a->getPos()+utils::math::polar2Cartesian(a->getAngle()+_argparams[i].a, _argparams[i].b);
	// int W = a->render()->bounds().width;
	// int H = a->render()->bounds().height;
	// int idx = arimod((int)offsetPos.y, H) * W + arimod((int) offsetPos.x, W);
	switch(p.type) {
		case 0: // return a defined constant
			return p.UNBOUND;
		case 1: // Return a random number from (-1,1)
			return utils::rand::rand_01()*2-1;
		case 2: // PHERMONE INTENSITY (squared) at offset
		case 3: // PHERMONE Activation by color
		{

			float ret = 0, rv;
			sf::Vector2f dat;
			for (int i = 0; i < v.size(); i++){
				// a->render()->pheromone().active[v[i]] = 1;
				dat = a->render()->pheromone().data[v[i]];
				if (p.type == 2)
					rv = utils::math::magsq(dat);
				else {
					// circular radian difference between two angles, normalized to (-1,1).
					rv = arfmod(std::atan2(dat.x, dat.y)-p.DIR2, M_PI*2);
					// printf("%f\n", d);
					rv = 1-std::min(rv, (float)M_PI*2-rv)/M_PI;
					// square to emphasize values near 1
					rv*=rv;
				}
				ret += rv*(MAX_SIGHT_RANGE*2.f*i/MAX_SIGHT - MAX_SIGHT_RANGE);
			}
			return ret / v.size();
		}
		case 4: // FOOD amount at offset
		{
			// return 0;
			//return magnitude squared of color vector at point
			float ret = 0;
			for (int i = 0; i < v.size(); i++) {
				ret += utils::math::magsq(a->render()->food().data[v[i]])*(MAX_SIGHT_RANGE*2.f*i/MAX_SIGHT - MAX_SIGHT_RANGE);
			}
			return ret / v.size();
		}
		case 5:
			return a->fullness();
	}
	return 0;
}

Network Network::random() {
	// printf("\nCREATING RANDOM BRAIN\n");

	int inputCount = utils::rand::binom(50, INITIAL_INPUT_CHANCE);
	int nodeCount = utils::rand::binom(50, INITIAL_NODE_CHANCE);
	// int nodeCount = 10;

	Network n(0, OUTPUT_NODE_COUNT, std::vector<Node>(nodeCount));

	for(int i = n._ocount; i < n._nodes.size(); i++) {
		n._nodes[i] = nUtils::RANDOM_NODE(n._nodes.size());
	}

	for(int i = 0; i < inputCount; i++) {
		n._inputs.push_back(nUtils::RANDOM_NODE(n._nodes.size()));
		n._argparams.push_back(nUtils::ArgParams());
	}

	// printf("Brain has %d inputs and %d nodes\n", inputCount, nodeCount);

	// n._inputs[0].children.push_back(0);
	// n._inputs[0].weights.push_back(1);
	// n._inputs[0].bias = -0.5;
	// n._nodes[0].children.push_back(n._N);
	// n._nodes[0].weights.push_back(1);

	// std::cout << "PRE PRUNE:" << n << std::endl;
	n.prune();
	// std::cout << "POST PRUNE:" << n << std::endl;
	return n;
}

void Network::mutate() {
	// std::cout << n << std::endl;
	int inputCount = utils::rand::binom(50, ADD_INPUT_CHANCE);
	int nodeCount = utils::rand::binom(50, ADD_NODE_CHANCE);
	// int inputCount = 0;
	// int nodeCount = 10;

	// std::vector<Node> newNodes;
	// printf("Adding %d nodes\n", nodeCount);

	int nsize = _nodes.size()+nodeCount;

	for(int i = 0; i < nodeCount; i++) {
		_nodes.push_back(nUtils::RANDOM_NODE(nsize));
		// newNodes.push_back(Node());
	}

	_states.resize(_nodes.size());

	// [<nodes>, ... , <output nodes>]

	// _nodes.insert(_nodes.begin()+_N, newNodes.begin(), newNodes.end());

	// printf("New size %ld\n", _nodes.size());

	// delete input nodes
	while (utils::rand::rand_01() < ADD_INPUT_CHANCE && _inputs.size() > 0) {
		int j = utils::rand::urand(0,_inputs.size());
		_inputs.erase(_inputs.begin()+j);
		_argparams.erase(_argparams.begin()+j);
	}

	// modify all inputs
	for(int i = 0; i < _inputs.size(); i++) {
		nUtils::MUTATE_ARG_PARAMS(_argparams[i]);
		nUtils::RANDOM_MUTATE(_inputs[i].bias);
		for (int j = 0; j < _inputs[i].children.size(); j++) {
			nUtils::RANDOM_MUTATE(_inputs[i].weights[j]);
		}

		while (utils::rand::rand_01() < REMOVE_CHILD_CHANCE && _inputs[i].children.size() > 0) {
			int j = utils::rand::urand(0,_inputs[i].children.size());
			// printf("J: %d in %ld\n", j, _inputs[i].children.size());
			_inputs[i].children.erase(_inputs[i].children.begin()+j);
			_inputs[i].weights.erase(_inputs[i].weights.begin()+j);
		}

		while (utils::rand::rand_01() < ADD_CHILD_CHANCE) {
			_inputs[i].children.push_back(utils::rand::urand(0,_nodes.size()));
			_inputs[i].weights.push_back(utils::rand::norm());
		}
	}

	//modify all nodes
	for(int i = _ocount; i < _nodes.size(); i++) {
		nUtils::RANDOM_MUTATE(_nodes[i].bias);
		for (int j = 0; j < _nodes[i].children.size(); j++) {
			nUtils::RANDOM_MUTATE(_nodes[i].weights[j]);
		}

		while (utils::rand::rand_01() < REMOVE_CHILD_CHANCE && _nodes[i].children.size() > 0) {
			int j = utils::rand::urand(0,_nodes[i].children.size());
			_nodes[i].children.erase(_nodes[i].children.begin()+j);
			_nodes[i].weights.erase(_nodes[i].weights.begin()+j);
		}

		while (utils::rand::rand_01() < ADD_CHILD_CHANCE) {
			_nodes[i].children.push_back(utils::rand::urand(0,_nodes.size()));
			_nodes[i].weights.push_back(utils::rand::norm());
		}
	}

	for(int i = 0; i < inputCount; i++) {
		_inputs.push_back(nUtils::RANDOM_NODE(_nodes.size()));
		_argparams.push_back(nUtils::ArgParams());
	}

	prune();

	// std::cout << "POST MUTATE: " << *this << std::endl;
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
			// if (_inputs[i].children.size() != _inputs[i].weights.size() || _inputs[i].children[j] > _nodes.size() || _inputs[i].children[j] < 0) {
			// 	std::cout << "SIZES: " << _inputs[i].children.size() << "\n" << _inputs[i].weights.size() << std::endl;
			// 	printf("%d, %d, %d\n", _inputs[i].children[j], i, j);
			// 	std::cout << *this << std::endl;
			// 	throw std::runtime_error("Fucked INPUT");
			// }
			// printf("%d, %d, %d\n", _inputs[i].children[j], i, j);
			_nodes[_inputs[i].children[j]].state += _inputs[i].state*_inputs[i].weights[j] + _inputs[i].bias;
			// printf("Propogating input to child %d with state %f\n", C,  _nodes[C].state);
		}
	}

	for(int i = _ocount; i < _nodes.size(); i++)
	{
		for (int j = 0; j < _nodes[i].children.size(); j++)
		{
			// if (_nodes[i].children.size() != _nodes[i].weights.size() || _nodes[i].children[j] > _nodes.size() || _nodes[i].children[j] < 0) {
			// 	std::cout << "SIZES: " << _nodes[i].children.size() << ", " << _nodes[i].weights.size() << std::endl;
			// 	printf("%d, %d, %d\n", _nodes[i].children[j], i, j);
			// 	std::cout << *this << std::endl;
			// 	throw std::runtime_error("Fucked");
			// }
			// printf("%d, %d, %d\n", _nodes[i].children[j], i, j);
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
		_inputs[i].state = parseArg(_argparams[i], a);
		if (std::isnan(_inputs[i].state)) {
			std::cout << *this << std::endl;
			throw std::runtime_error("NaN input DETECTED!!! Aborting...");
		}
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
		os << "INPUT NODE " << i << "\n";
		os << "TYPE: " << n._argparams[i].type << " with params DIR1: " << n._argparams[i].DIR1 << " R01: " << n._argparams[i].R01  << " DIR2: " << n._argparams[i].DIR2 << " UNBOUND: " << n._argparams[i].UNBOUND << "\n";
		os << "NODE has bias " << n._inputs[i].bias << " and is at state " << n._inputs[i].state << " and the following children:\n";
		for (int j = 0; j < n._inputs[i].children.size(); j++) {
			os << "Weights length " <<  n._inputs[i].weights.size() << ", children length " << n._inputs[i].children.size() << "\n";
			os << "\tCHILD " << n._inputs[i].children[j] << " with weight " << n._inputs[i].weights[j] << "\n";
		}
	}

	for(int i = n._ocount; i < n._nodes.size(); i++) {
		os << "NODE " << i << " has bias " << n._nodes[i].bias  << " and is at state " << n._nodes[i].state << " and the following children:\n";
		for (int j = 0; j < n._nodes[i].children.size(); j++) {
			os << "\tCHILD " << n._nodes[i].children[j] << " with weight " << n._nodes[i].weights[j] << "\n";
		}
	}

	for(int i = 0; i < n._ocount; i++) {
		os << "OUTPUT NODE " << i << " has state " << n._nodes[i].state << "\n";
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