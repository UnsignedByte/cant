/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:55
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 19:05:15
*/

#include "rtneat.hpp"
#include "utils.hpp"

void Network::propogate()
{
	for(int i = 0; i < _N; i++)
	{
		_states[i] = _nodes[i].state;
	}

	for(int i = 0; i < _inputs.size(); i++)
	{
		for (Node* C : _nodes[i].children)
		{
			C->state *= utils::math::sigmoid(_nodes[i].state + _nodes[i].bias);
		}
	}

	for(int i = 0; i < _N; i++)
	{
		for (Node* C : _nodes[i].children)
		{
			C->state *= utils::math::sigmoid(_states[i] + _nodes[i].bias);
		}
	}
}


std::vector<float> Network::tick(std::vector<float> inputs)
{
	for(int i = 0; i < _inputs.size(); i++)
	{
		_inputs[i].state = inputs[i];
	}

	propogate();

	std::vector<float> outputs(_outputs.size());
	for(int i = 0; i < _outputs.size(); i++)
	{
		outputs[i] = _outputs[i].state;
	}
	return outputs;
}