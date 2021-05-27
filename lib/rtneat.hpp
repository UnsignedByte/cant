/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:48
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-27 09:42:15
*/

#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>
#include "utils.hpp"

struct Ant;

//max distance ants can see or smell
const float MAX_SIGHT = 3;

const int OUTPUT_NODE_COUNT = 3;

const float INITIAL_INPUT_CHANCE = 0.9;
const float INITIAL_NODE_CHANCE = 0.95;
const float INITIAL_CHILD_CHANCE = 0.6;

const float REMOVE_CHILD_CHANCE = 0.05;
const float ADD_CHILD_CHANCE = 0.05;
const float MUTATE_VALUE = 0.01;
const float MUTATE_FLIP = 0.05;
const float MUTATE_RESET = 0.02;
const float ADD_NODE_CHANCE = 0.005;
const float ADD_INPUT_CHANCE = 0.002;

struct Node
{
	Node() = default;

	Node(std::vector<int> children, std::vector<float> weights, float state, float bias): children(children), weights(weights), state(state), bias(bias) {}
	std::vector<int> children;
	std::vector<float> weights;
	float state = 0;
	float bias = 0;
};

namespace nUtils {
	struct ArgParams
	{
		int type = 0;
		float offsetDir = 0;
		float offsetMag = 1;
		float matchDir = 0;
	};

	void RANDOM_MUTATE(float& f, bool allowFlip = true, float max = std::numeric_limits<float>::infinity());

	ArgParams RANDOM_PARAMS();

	Node RANDOM_NODE(int);
}

struct Network
{
public:
	Network(int icount, int ocount): _ocount(ocount)
	{
		_inputs.resize(icount);
		_argparams.resize(icount);
	}

	Network(int icount, int ocount, std::vector<Node> nodes): Network(icount, ocount)
	{
		_nodes = nodes;
		_states.resize(nodes.size());
		_nodes.resize(nodes.size()+ocount);
	}

	void mutate()
	{
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

		// modify all inputs
		for(int i = 0; i < _inputs.size(); i++) {
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
			_argparams.push_back(nUtils::RANDOM_PARAMS());
		}

		prune();

		// std::cout << "POST MUTATE: " << *this << std::endl;
	}

	static Network random()
	{
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
			n._argparams.push_back(nUtils::RANDOM_PARAMS());
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

	void prune();

	int N() const;

	float output(int) const;

	void tick(Ant*);

	void propogate();

	friend std::ostream& operator<<(std::ostream &os, const Network& n);
private:
	bool _prune_prop(int i, bool* visited, bool* dead);
	float parseArg(int, Ant*) const;
	int _ocount;
	std::vector<Node> _inputs;
	std::vector<Node> _nodes;
	std::vector<float> _states;
	std::vector<nUtils::ArgParams> _argparams;
};