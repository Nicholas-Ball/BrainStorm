#include <iostream>
#include <vector>
#include "../nlohmann/json.hpp"

namespace Brainstorm {
	enum Types {SIGMOID,RELU,LRELU}; //neuron activation types
	class Neuron
	{
		public:
			double bias;
			std::vector<double> weights;
			bool Input = false;
			double output;
			std::vector<double> deltas;
			double cost = 0;
			double preActivation = 0;
			std::vector<Neuron> Inputs;

			//Save as json
			nlohmann::json Save()
			{
				nlohmann::json j;

				j["bias"] = this->bias;
				j["weights"] = this->weights;
				j["input"] = this->Input;
				j["output"] = this->output;

				return j;
			}

			//load data from json
			void Load(nlohmann::json data)
			{
				this->bias = data["bias"];
				this->weights = data["weights"].get<std::vector<double>>();
				this->Input = data["input"].get<bool>();
				this->output = data["output"];
			}
	}; 
};