/*
* @Author: UnsignedByte
* @Date:   2021-05-24 10:13:48
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-05-24 19:13:29
*/

#include <vector>

struct Node
{
	std::vector<Node*> children;
	std::vector<float> weights;
	float state = 0;
	float bias = 0;
};

struct Network
{
public:
	Network(int icount, int ocount)
	{
		_inputs.resize(icount);
		_outputs.resize(ocount);
	}

	Network(int icount, int ocount, std::vector<Node> nodes): Network(icount, ocount)
	{
		_nodes = nodes;
		_N = nodes.size();
		_states.resize(_N);
	}

	std::vector<float> tick(std::vector<float> inputs);

	void propogate();
private:
	int _N;
	std::vector<Node> _inputs;
	std::vector<Node> _nodes;
	std::vector<float> _states;
	std::vector<Node> _outputs;
};