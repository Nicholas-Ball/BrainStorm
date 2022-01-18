#include <iostream>
#include <vector>

namespace Brainstorm {
	enum Types {SIGMOID};
	class Neuron
	{
		public:
			double bias;
			std::vector<double> weights;
			bool Input = false;
			double output;
			std::vector<Neuron> Inputs;
	}; 
};