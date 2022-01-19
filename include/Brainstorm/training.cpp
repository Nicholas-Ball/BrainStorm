#include "training.hpp"


//train network with specified type
void Brainstorm::Training::Train(Brainstorm::FeedForward network,TrainingType t)
{
    if(t == TrainingType::NatrualSelection)
    {
        NatrualSelectionFF(network);
    }
}

//add training data
void Brainstorm::Training::AddTrainingData(std::vector<double> input,std::vector<double> expectedOutput)
{
    this->Inputs.push_back(input);
    this->ExcpectedOutputs.push_back(expectedOutput);
}

//feed forward natural selection training
