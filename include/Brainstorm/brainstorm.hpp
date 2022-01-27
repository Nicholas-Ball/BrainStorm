#include <iostream>
#include <vector>
#include <time.h> 
#include <thread>
#include "neuron.hpp"
#include "math.hpp"
#include "../img/img.hpp"

namespace Brainstorm {

	class FeedForward
	{
		private:
			std::vector<double> result; //array of outputs from last layer
			Brainstorm::Types type = Brainstorm::SIGMOID; // type of network like RELU or SIGMOID
			double (*function)(double) = math::Sigmoid; //mathmatical function of current activation function
			double (*dfunction)(double) = math::Sigmoid; //mathmatical derivative function of current activation function
			

		public:
			std::vector<std::vector<Brainstorm::Neuron>> network; //network array

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
			
			//save network as json;
			nlohmann::json Save();

			//load network from json
			void Load(nlohmann::json data);

			//returns pointer to raw network
			std::vector<std::vector<Brainstorm::Neuron>>* GetNetwork();
			
			//picks random nueron in random layer to randomize weight
			void Randomize();

			double Derivative(double x)
			{
				return dfunction(x);
			}

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
				this->error = f.error;
			}

	};
	
	class CNN
	{
		private:
			std::vector<std::vector<std::vector<double>>> Scans; //convolutional scan patterns
			Brainstorm::FeedForward FFNetwork; //feedforwad network
			int Pool = 2; //number of times the network preforms conolution and pools the image
		public:
			void Run(Img image); //run cnn network

			std::vector<double> GetOuput(); //get output of network

			void Generate(std::vector<int> matrix); //generate network and pass matrix for feed forward network

			void AddScan(std::vector<std::vector<double>> scan);
	}; 
};