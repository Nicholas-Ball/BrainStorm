#include <iostream>
#include <vector>
#include <time.h> 
#include <thread>
#include "neuron.hpp"
#include "math.hpp"

namespace Brainstorm {
	class FeedForward
	{
		private:
			std::vector<std::vector<Brainstorm::Neuron>> network; //network array
			Brainstorm::Types type = Brainstorm::SIGMOID; // type of network like RELU or SIGMOID
			double (*function)(double) = math::Sigmoid; //mathmatical function of current activation function
			std::vector<double> result; //array of outputs from last layer
			

		public:
			//error of network
			double error = 0;
			//return network output as array
			std::vector<double> GetOutput();

			//Set type of network like RELU or SIGMOID
			void SetType(Brainstorm::Types t);

			//Generate the feed forward network
			void Generate(std::vector<int> neuronMatrix);

			//run the feed forward network
			void Run(std::vector<double> input);

			//returns pointer to raw network
			std::vector<std::vector<Brainstorm::Neuron>>* GetNetwork();
			
			//picks random nueron in random layer to randomize weight
			void Randomize();

			FeedForward()
			{
   				//set seed
   				srand(time(NULL) + std::hash<std::thread::id>{}(std::this_thread::get_id()));
			}
			

			//copy constructor
			FeedForward(const FeedForward &f)
			{
   			//set seed
   			srand(time(NULL) + std::hash<std::thread::id>{}(std::this_thread::get_id()));
				   
				//copy core meta data
				this->network = f.network;
				this->type = f.type;
				this->function = f.function;
				this->result = f.result;
			}

	}; 
};