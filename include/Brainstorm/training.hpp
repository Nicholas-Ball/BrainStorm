#include "brainstorm.hpp"
#include <functional>
#include <algorithm>
#include <map>
#include <math.h>

int count = 0;

namespace Brainstorm{

    class Training
    {
        private:
            double SurvivalRate;
            double TrainingRate = 0.1;
            int NumCreatures,Epoches;
            bool Verbose,Automated = false;
            std::vector<std::vector<double>> Inputs,ExcpectedOutputs;


        public:
            enum TrainingType {NatrualSelection,BackPropagation};

            //natural selection
            void SetSurvivalRate(double rate);
            void SetNumberOfCreatures(int num);
            void SetEpoches(int epoches);
            void SetVerbose(bool v);
            Brainstorm::FeedForward Train(Brainstorm::FeedForward network, TrainingType t);
            void AddTrainingData(std::vector<double> input, std::vector<double> expectedOutput);
            Brainstorm::FeedForward NatrualSelectionFF(Brainstorm::FeedForward network);

            //backpropagation
            void SetTrainingRate(double x); //manually set training rate
            void SetAutomated(bool a); //automatically set training rate
            Brainstorm::FeedForward BackPropagationFF(Brainstorm::FeedForward network);
    };
}

bool ffCompare(Brainstorm::FeedForward c1, Brainstorm::FeedForward c2)
{
    return c1.error < c2.error;
}

//Set survival rate (EX: 0.25 == 25%)
void Brainstorm::Training::SetSurvivalRate(double rate)
{
    this->SurvivalRate = rate;
}

//number of creatues running at once (EX: 1000 == 1000 spawned creatures)
void Brainstorm::Training::SetNumberOfCreatures(int num)
{
    this->NumCreatures = num;
}

//number of epoches the training data will be runned (EX: 100 == 100 runs)
void Brainstorm::Training::SetEpoches(int epoches)
{
    this->Epoches = epoches;
}

//turn on / off verbose (This will print out data about the current training)
void Brainstorm::Training::SetVerbose(bool v)
{
    this->Verbose = v;
}

//automatically set training rate (true/false)
void Brainstorm::Training::SetAutomated(bool a){
    this->Automated = a;
} 

//train network with specified type
Brainstorm::FeedForward Brainstorm::Training::Train(Brainstorm::FeedForward network, TrainingType t)
{
    if (t == TrainingType::NatrualSelection)
    {
        network = NatrualSelectionFF(network);
        return network;
    }
    else if (t == TrainingType::BackPropagation)
    {
        network = BackPropagationFF(network);
        return network;
    }
    return network;
}

//add training data
void Brainstorm::Training::AddTrainingData(std::vector<double> input, std::vector<double> expectedOutput)
{
    this->Inputs.push_back(input);
    this->ExcpectedOutputs.push_back(expectedOutput);
}

std::vector<Brainstorm::FeedForward> networks;

//run network and calc error for feed forward network
void static RunAndCalc(int ID, std::vector<std::vector<double>>* input, std::vector<std::vector<double>>* expectedOutput)
{
    networks[ID].error = 0;

    //run through input data and add calculate error
    for (int i = 0; i != input->size(); i++)
    {

        //run network
        networks[ID].Run(input->at(i));

        //get network output
        auto out = networks[ID].GetOutput();

        //calculate error and add it to error total
        for (int o = 0; o != out.size(); o++)
        {
            networks[ID].error += (out[o] - expectedOutput->at(i)[o]) * (out[o] - expectedOutput->at(i)[o]);
        }
    }

}

//feed forward natural selection training
Brainstorm::FeedForward Brainstorm::Training::NatrualSelectionFF(Brainstorm::FeedForward network)
{
    //array of creature networks
    networks = { network };

    //number of creatures to have at one time
    int numCreatures = this->NumCreatures;

    //set the natural selection survival rate
    double survivalRate = this->SurvivalRate;



    //run through Epoches
    for (int e = 0; e != this->Epoches; e++)
    {
        //array of threads to later rejoin them
        std::vector<std::thread> threads;


        threads.push_back(std::thread(RunAndCalc, networks.size() - 1, &this->Inputs, &this->ExcpectedOutputs));
        //generate networks and run them on a thread
        while (networks.size() != numCreatures)
        {
            //create new creature with a survived creature
            Brainstorm::FeedForward newC(networks[networks.size() - 1 % ((int)(numCreatures * survivalRate) - 1)]);

            //randomize network
            newC.Randomize();

            networks.push_back(newC);

            threads.push_back(std::thread(RunAndCalc, networks.size() - 1, &this->Inputs, &this->ExcpectedOutputs));

        }

        //rejoin threads
        for (int t = 0; t != threads.size(); t++)
        {
            threads[t].join();
        }

        //sort error smallest to biggest
        std::sort(networks.begin(), networks.end(), ffCompare);


        //make data to remove creatures that didn't survive
        std::vector<Brainstorm::FeedForward> temp = {};

        //remove all creatures that didn't survive
        for (int n = 0; n != networks.size() * survivalRate; n++)
        {
            temp.push_back(networks[n]);
        }

        //set data of survived creatures
        networks = temp;



        if (this->Verbose)
        {
            //print best network that isn't nan if in verbose mode
            for (int n = 0; n != networks.size(); n++)
            {
                if (networks[n].error != 0)
                {
                    std::cout << "Epoch " << e << " error: " << networks[n].error << std::endl;
                    break;
                }
            }
        }
    }

    //return best network that isn't nan
    for (int n = 0; n != networks.size(); n++)
    {
        if (networks[n].error != 0)
        {
            return networks[n];
        }
    }
    return networks[0];
}


//Set training rate (EX: 0.1)
void Brainstorm::Training::SetTrainingRate(double x)
{
    this->TrainingRate = x;
}


Brainstorm::FeedForward Brainstorm::Training::BackPropagationFF(Brainstorm::FeedForward network)
{
    //get learning rate
    double learningRate = this->TrainingRate;

    //previous error
    double pError = 100000000000000000000000000000000000000000000.0;

    //used to stop training under automated
    bool keepGoing = this->Automated;

    //count the number of same errors to end early
    int same = 0;

    for(int x = 0; x != this->ExcpectedOutputs.size();x++){
        for(int y = 0; y != this->ExcpectedOutputs[x].size();y++)
        {
            std::cout<<this->ExcpectedOutputs[x][y]<<" ";
        }
        
        std::cout<<std::endl;
    }

    //loop through epoches
    for (int e = 0; e != this->Epoches || keepGoing; e++)
    {
        network.error = 0;
        
        //loop through training data
        for (int t = 0; t != this->Inputs.size(); t++)
        {
            network.Run(this->Inputs[t]);
            auto out = network.GetOutput();

            //calculate cost
            for (int o = 0; o != out.size(); o++)
            {
                network.error += (out[o] - this->ExcpectedOutputs[t][o])*(out[o] - this->ExcpectedOutputs[t][o]);
            }

            //calculate deltas
            //loop through layers backwards
            for (int l = network.network.size() - 1; l != 0; l--)
            {
                //loop through neurons in layer
                for (int n = 0; n != network.network[l].size(); n++)
                {
                    //loop through weights of a neuron
                    for (int w = 0; w != network.network[l][n].weights.size(); w++)
                    {
                        //if on last layer use network error 
                        if (l == network.network.size() - 1)
                        {

                            // dCost = 2(output - expected)
                            double dCost = 2 * (network.network[l][n].output - this->ExcpectedOutputs[t][n]);

                            //get previous output
                            double pre = network.network[l - 1][w].output;

                            //derivate of function
                            double dOut = network.Derivative(network.network[l][n].preActivation,l);

                            //delta = (currentdelta + (dCost*preOutput*dOut)) / 2
                            network.network[l][n].deltas[w] += (dCost * pre * dOut);
                            network.network[l][n].deltas[w] /= (1 + (t != 0));

                        }
                        else {
                            //get previous output
                            double pre = network.network[l - 1][w].output;

                            //derivate of function
                            double dOut = network.Derivative(network.network[l][n].preActivation,l);

                            network.network[l][n].deltas[w] += (pre * dOut);
                            network.network[l][n].deltas[w] /= (1 + (t != 0));
                        }

                    }

                }
            }

        }

        if (this->Verbose) std::cout << "Epoch " << e << " Error: " << network.error << std::endl;

        //for automated adjustment of learning rate
        if(this->Automated)
        {
            //get current error
            double cError = network.error;

            //decrease learning rate if error is increasing
            if(pError <=  cError)
            {
                learningRate /= 10;
            }

            if(pError==cError)
                same++;
            else
                same = 0;

            if((cError < 0.0001) || (same == 5))
            {
                break;
            }

        }

        pError = network.error;
        
        network.error = 0;

        //update weights
        //loop through layers
        for (int l = 1; l != network.network.size(); l++)
        {
            //loop through neurons
            for (int n = 0; n != network.network[l].size(); n++)
            {
                //loop through weights
                for (int w = 0; w != network.network[l][n].weights.size(); w++)
                {
                    //set new weight
                    network.network[l][n].weights[w] -= (learningRate * network.network[l][n].output * network.network[l][n].deltas[w]);

                    //set new bias
                    //network.network[l][n].bias -= (learningRate * 1 * network.network[l][n].deltas[w]);


                    //reset delta
                    network.network[l][n].deltas[w] = 0;
                }
            }
        }

    }
    return network;
}