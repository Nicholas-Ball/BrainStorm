/*
    Feed forward network file
*/

#include "brainstorm.hpp"
#include "../nlohmann/json.hpp"
#include "../img/img.hpp"

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

    //save network type
    if(this->type == Brainstorm::Types::SIGMOID)
    {
        j["type"] = 0;
    } else if(this->type == Brainstorm::Types::RELU)
    {
        j["type"] = 1;
    }else if(this->type == Brainstorm::Types::LRELU)
    {
        j["type"] = 2;
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

    //set network type
    if(data["type"] == 0)
    {
        this->SetType(Brainstorm::Types::SIGMOID);
    } else if(data["type"] == 1)
    {
        this->SetType(Brainstorm::Types::RELU);
    }else if(data["type"] == 2)
    {
        this->SetType(Brainstorm::Types::LRELU);
    }
}

//generate feed forward network
void Brainstorm::FeedForward::Generate(std::vector<int> neuronMatrix)
{
    //create layer
    for(int l = 0; l != neuronMatrix.size();l++)
    {
        std::vector<Brainstorm::Neuron> layer;
        //create neurons
        for(int n = 0; n != neuronMatrix[l];n++)
        {
            //vreate new neuron
            Brainstorm::Neuron newNeuron;

            //set defualt data
            newNeuron.bias = 1;
            newNeuron.Input = (l == 0);
            newNeuron.output = 0;
            
            //create weights
            for(int w = 0; l != 0 && w != neuronMatrix[l-1];w++)
            {
                newNeuron.weights.push_back(1.0);
                newNeuron.deltas.push_back(0.0);
            }

            //add to layer
            layer.push_back(newNeuron);
        }

        //add to network
        this->network.push_back(layer);
    }

}

void Brainstorm::FeedForward::SetType(Types t)
{
    //set current type of function
    this->type = t;
    
    //set activation function
    if(t == Types::SIGMOID)
    {
        this->function = math::Sigmoid;
        this->dfunction = math::dSigmoid;
    }
    else if(t == Types::RELU)
    {
        this->function = math::RELU;
        this->dfunction = math::dRELU;
    }
    else if(t == Types::LRELU)
    {
        this->function = math::LRELU;
        this->dfunction = math::dLRELU;
    }
    else{
        std::cout<<"Invaild type"<<std::endl;
        std::abort();
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

           this->network[l][n].output = this->function(calc+this->network[l][n].bias);
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


