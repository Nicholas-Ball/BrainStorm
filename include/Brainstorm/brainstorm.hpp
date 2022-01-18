#include <iostream>
#include <vector>
#include "neuron.hpp"

namespace Brainstorm {
	class FeedForward
	{
		private:
			std::vector<std::vector<Neuron>> network;
			Type type;
			

		public:
			void SetType(Type t);
			void Generate(int numLayer,std::vector<int> neuronMatrix);

			void Run

	}; 
};