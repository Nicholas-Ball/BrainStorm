/*
    Feed forward network file
*/

#include "brainstorm.hpp"

//return network output as array
std::vector<double> Brainstorm::FeedForward::GetOutput()
{
    return this->result;
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
    }
    else if(t == Types::RELU)
    {
        this->function = math::RELU;
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
           temp.push_back(this->function(calc+this->network[l][n].bias)); //put data+bias through activation function and add to temp for later use
        }
        //set next input data
        input = temp;
    }

    //set result of the output of last layer
    this->result = input;
}