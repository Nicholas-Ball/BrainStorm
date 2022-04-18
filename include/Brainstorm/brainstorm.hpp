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
			std::vector<double> result; //array of outputs from last layer
			std::vector<Brainstorm::Types> types = {Brainstorm::Types::RELU}; // type of network like RELU or SIGMOID with array refering to each layer
			double Calc(double x,int layer); //mathmatical function of current activation function
			double dCalc(double x,int layer); //mathmatical derivative function of current activation function
			

		public:
			std::vector<std::vector<Brainstorm::Neuron>> network; //network array

			//error of network
			double error = 0;


			//return network output as array
			std::vector<double> GetOutput();

			//Generate the feed forward network
			void Generate(std::vector<int> neuronMatrix,std::vector<Brainstorm::Types> functions);
			void Generate(std::vector<int> neuronMatrix,Brainstorm::Types function);

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

			double Derivative(double x,int layer)
			{
				return dCalc(x,layer);
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
				this->types = f.types;
				this->result = f.result;
				this->error = f.error;
			}

	};
};

double Brainstorm::FeedForward::Calc(double x,int layer)
{
    //get layer type
    auto type = this->types[layer];

    
    //preform activation function and return it
    if(type == Brainstorm::Types::SIGMOID)
    {
        return math::Sigmoid(x);
    } else if(type == Brainstorm::Types::RELU)
    {
        return math::RELU(x);
    }else if(type == Brainstorm::Types::LRELU)
    {
        return math::LRELU(x);
    }else if(type == Brainstorm::Types::TANH)
    {
        return math::TANH(x);
    }
    return -1;

}

double Brainstorm::FeedForward::dCalc(double x,int layer)
{
    //get layer type
    auto type = this->types[layer];
    
    //get derivative of activation function and return it
    if(type == Brainstorm::Types::SIGMOID)
    {
        return math::dSigmoid(x);
    } else if(type == Brainstorm::Types::RELU)
    {
        return math::dRELU(x);
    }else if(type == Brainstorm::Types::LRELU)
    {
        return math::dLRELU(x);
    }else if(type == Brainstorm::Types::TANH)
    {
        return math::dTANH(x);
    }
    return -1;
}

//return network output as array
std::vector<double> Brainstorm::FeedForward::GetOutput()
{
    return this->result;
}

//save network to json data
nlohmann::json Brainstorm::FeedForward::Save()
{
    nlohmann::json j;

    //save network
    for(int l = 0; l != this->network.size();l++)
    {
        for(int n = 0; n != this->network[l].size();n++)
        {
            j["network"][l][n] = this->network[l][n].Save();
        }
    }
    //save type by layer
    for(int i = 0; i != this->types.size();i++)
    {
        //save network type
        if(this->types[i] == Brainstorm::Types::SIGMOID)
        {
            j["types"][i] = 0;
        } else if(this->types[i] == Brainstorm::Types::RELU)
        {
            j["types"][i] = 1;
        }else if(this->types[i] == Brainstorm::Types::LRELU)
        {
            j["types"][i] = 2;
        }else if(this->types[i] == Brainstorm::Types::TANH)
        {
            j["types"][i] = 3;
        }

    }


    return j;
}

//load network from json data
void Brainstorm::FeedForward::Load(nlohmann::json data)
{
    //load network
    for(int l = 0; l != data["network"].size();l++)
    {
        std::vector<Neuron> layer;
        for(int n = 0; n != data["network"][l].size();n++)
        {
            //add neuron to layer
            Neuron ne;
            ne.Load(data["network"][l][n]);
            layer.push_back(ne);
        }
        //add layer to network
        this->network.push_back(layer);
    }

    //reset network types
    this->types = {};

    for(int i = 0; i != data["types"].size();i++)
    {
        //set network type
        if(data["types"][i] == 0)
        {
            this->types.push_back(Brainstorm::Types::SIGMOID);
        } else if(data["types"] == 1)
        {
            this->types.push_back(Brainstorm::Types::RELU);
        }else if(data["types"] == 2)
        {
            this->types.push_back(Brainstorm::Types::LRELU);
        }else if(this->types[i] == 3)
        {
            this->types.push_back(Brainstorm::Types::TANH);
        }

    }

}

//generate feed forward network with all same type
void Brainstorm::FeedForward::Generate(std::vector<int> neuronMatrix,Brainstorm::Types function){
    //make array of types
    std::vector<Brainstorm::Types> functions;

    //set all functions as one function
    for(int i = 0; i != neuronMatrix.size();i++)
    {
        functions.push_back(function);
    }

    //generate
    this->Generate(neuronMatrix,functions);
}

//generate feed forward network with unique types
void Brainstorm::FeedForward::Generate(std::vector<int> neuronMatrix,std::vector<Brainstorm::Types> functions)
{
    //create layer
    for(int l = 0; l != neuronMatrix.size();l++)
    {
        std::vector<Brainstorm::Neuron> layer;

        //set activation type 
        this->types.push_back(functions[l]);

        //create neurons
        for(int n = 0; n != neuronMatrix[l];n++)
        {
            //create new neuron
            Brainstorm::Neuron newNeuron;

            //set defualt data
            newNeuron.bias = 0;
            newNeuron.Input = (l == 0);
            newNeuron.output = 0;
            
            //create weights
            for(int w = 0; l != 0 && w != neuronMatrix[l-1];w++)
            {
                newNeuron.weights.push_back((rand() % 10) - 5);
                newNeuron.deltas.push_back(0.0);
            }

            //add to layer
            layer.push_back(newNeuron);
        }

        //add to network
        this->network.push_back(layer);
    }

}


void Brainstorm::FeedForward::Run(std::vector<double> input)
{
    //loop through layers
    for(int l = 0; l != this->network.size();l++)
    {
        std::vector<double> temp;
        //loop through neurons in layer
        for(int n = 0; n != this->network[l].size();n++)
        {
            double calc = 0.0; //used for activation function
            //loop through input data and multiply by weight
            for(int w = 0; w != input.size();w++)
            {

                //if there are more inputs than weights, add a new weight
                if(w == this->network[l][n].weights.size())
                {
                    this->network[l][n].weights.push_back(1);
                }
                calc += input[w]* this->network[l][n].weights[w];
            }

           this->network[l][n].output = this->Calc(calc+this->network[l][n].bias,l);
           temp.push_back(this->network[l][n].output); //put data+bias through activation function and add to temp for later use
           this->network[l][n].preActivation = calc+this->network[l][n].bias;
        }

        //set next input data
        input = temp;
    }

    //set result of the output of last layer
    this->result = input;
}

std::vector<std::vector<Brainstorm::Neuron>>* Brainstorm::FeedForward::GetNetwork()
{
    return &this->network;
}

//picks random nueron in random layer to randomize weight
void Brainstorm::FeedForward::Randomize()
{
    //pick layer
    int layer = rand() % (this->network.size()-1);

    //pick neuron
    int neuron = rand() % (this->network[layer].size());

    //randomize weights
    for(int w = 0; w != this->network[layer][neuron].weights.size();w++)
    {
        this->network[layer][neuron].weights[w] += (rand() % 10) - 5;
    }
}


