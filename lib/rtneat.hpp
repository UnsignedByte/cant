/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:48
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-25 08:22:37
*/

#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <cstdio>
#include <cmath>
#include "utils.hpp"

struct Ant;

const float ADD_INPUT_CHANCE = 0.8;
const float ADD_NODE_CHANCE = 0.8;
const float ADD_CHILD_CHANCE = 0.8;

struct Node
{
	Node() = default;

	Node(std::vector<int> children, std::vector<float> weights, float state, float bias): children(children), weights(weights), state(state), bias(bias) {}
	std::vector<int> children;
	std::vector<float> weights;
	float state = 0;
	float bias = 0;
};

struct ArgParams
{
	ArgParams() = default;
	int type = 0;
	float offsetDir = 0;
	float offsetMag = 1;
	float matchDir = 0;
};

struct Network
{
public:
	Network(int icount)
	{
		_inputs.resize(icount);
		_argparams.resize(icount);
	}

	Network(int icount, int ocount, std::vector<Node> nodes): Network(icount)
	{
		_nodes = nodes;
		_N = nodes.size();
		_states.resize(_N);
		_nodes.resize(_N+ocount);
	}

	static Network random()
	{
		printf("\nCREATING RANDOM BRAIN\n");
		Network n(0, 2, std::vector<Node>());

		while (utils::rand::rand_01() < ADD_INPUT_CHANCE) {
			n._inputs.push_back(Node());
			n._argparams.push_back(ArgParams());
			n._argparams.back().type = utils::rand::urand(0,3);
			switch(n._argparams.back().type) {
				case 0:
					printf("Creating random parameter\n");
					break;
				case 2:
					n._argparams.back().matchDir = utils::rand::rand_01()*M_PI*2;
				case 1:
					n._argparams.back().offsetDir = utils::rand::rand_01()*M_PI*2-M_PI;
					n._argparams.back().offsetMag = utils::rand::norm();
					printf("Creating parameter type %d with offset (%f, %f) and match %f\n", n._argparams.back().type, n._argparams.back().offsetDir, n._argparams.back().offsetMag, n._argparams.back().matchDir);
					break;
			}
		}

		while (utils::rand::rand_01() < ADD_NODE_CHANCE) {
			n._nodes.push_back(Node());
			n._nodes.back().bias = utils::rand::norm();
		}

		printf("Brain has %ld inputs and %d nodes\n", n._inputs.size(), n._N);

		for(int i = 0; i < n._inputs.size(); i++) {
			while (utils::rand::rand_01() < ADD_CHILD_CHANCE) {
				n._inputs[i].children.push_back(utils::rand::urand(0,n._nodes.size()));
				n._inputs[i].weights.push_back(utils::rand::norm());
			}
		}

		for(int i = 0; i < n._nodes.size(); i++) {
			while (utils::rand::rand_01() < ADD_CHILD_CHANCE) {
				n._nodes[i].children.push_back(utils::rand::urand(0,n._nodes.size()));
				n._nodes[i].weights.push_back(utils::rand::norm());
			}
		}

		// n._inputs[0].children.push_back(0);
		// n._inputs[0].weights.push_back(1);
		// n._inputs[0].bias = -0.5;
		// n._nodes[0].children.push_back(n._N);
		// n._nodes[0].weights.push_back(1);
		return n;
	}

	float output(int) const;

	void tick(Ant*);

	void propogate();
private:
	float parseArg(int, Ant*) const;
	int _N;
	std::vector<Node> _inputs;
	std::vector<Node> _nodes;
	std::vector<float> _states;
	std::vector<ArgParams> _argparams;
};