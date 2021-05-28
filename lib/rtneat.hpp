/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:48
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-28 01:05:28
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
const float MAX_SIGHT = 10;
// max view range of this fraction 360 degrees in each direction
const float MAX_SIGHT_RANGE = .1f/.8f;

const int OUTPUT_NODE_COUNT = 3;

const float INITIAL_INPUT_CHANCE = 0.8;
const float INITIAL_NODE_CHANCE = 0.8;
const float INITIAL_CHILD_CHANCE = 0.8;

const float REMOVE_CHILD_CHANCE = 0.005;
const float ADD_CHILD_CHANCE = 0.005;
const float MUTATE_VALUE = 0.01;
const float MUTATE_FLIP = 0.05;
const float MUTATE_RESET = 0.02;
const float ADD_NODE_CHANCE = 0.001;
const float ADD_INPUT_CHANCE = 0.001;

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
		ArgParams();
		int type;
		float DIR1, R01, DIR2, UNBOUND;
	};

	void RANDOM_MUTATE(float& f, bool allowFlip = true, float max = std::numeric_limits<float>::infinity());
	void RANDOM_MUTATE_THETA(float& f);

	void MUTATE_ARG_PARAMS(ArgParams& a);

	std::vector<int> POINTS_ON_ARC(Ant* a, const float tmin, const float tmax, const float dist, const float sparsity = 0.25f);

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

	void mutate();

	static Network random();

	void prune();

	int N() const;

	float output(int) const;

	void tick(Ant*);

	void propogate();

	friend std::ostream& operator<<(std::ostream &os, const Network& n);
private:
	bool _prune_prop(int i, bool* visited, bool* dead);
	float parseArg(const nUtils::ArgParams&, Ant*) const;
	int _ocount;
	std::vector<Node> _inputs;
	std::vector<Node> _nodes;
	std::vector<float> _states;
	std::vector<nUtils::ArgParams> _argparams;
};