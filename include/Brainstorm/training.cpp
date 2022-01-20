#include "training.hpp"
#include <functional>


//train network with specified type
void Brainstorm::Training::Train(Brainstorm::FeedForward network,TrainingType t,int Epoches)
{
    if(t == TrainingType::NatrualSelection)
    {
        NatrualSelectionFF(network,Epoches);
    }
}

//add training data
void Brainstorm::Training::AddTrainingData(std::vector<double> input,std::vector<double> expectedOutput)
{
    this->Inputs.push_back(input);
    this->ExcpectedOutputs.push_back(expectedOutput);
}

//run network and calc error for feed forward network
void static RunAndCalc(Brainstorm::FeedForward network,std::vector<std::vector<double>> input,std::vector<std::vector<double>> expectedOutput,std::vector<double>& errorArray,int errorID)
{
    double error = 0;

    std::cout<<"test1"<<std::endl;

    //run through input data and add calculate error
    for(int i = 0;i != input.size();i++)
    {
        //run network
        network.Run(input[i]);

        //get network output
        auto out = network.GetOutput();

        //calculate error and add it to error total
        for(int o = 0; o != out.size();o++)
        {
            error += (expectedOutput[i][o] - out[o])*(expectedOutput[i][o] - out[o]);
        }
    }
    
    std::cout<<"test2"<<std::endl;

    errorArray[errorID] = error;
    std::cout<<"test3"<<std::endl;

}

//feed forward natural selection training
void Brainstorm::Training::NatrualSelectionFF(Brainstorm::FeedForward network,int Epoches)
{
    //array of creature networks
    std::vector<Brainstorm::FeedForward> networks = {network};

    //number of creatures to have at one time
    int numCreatures = 100;

    double survivalRate = 0.25;

    std::vector<double> errorArray;

    //run through Epoches
    for(int e = 0; e != Epoches;e++)
    {
        std::cout<<e<<std::endl;
        
        //generate networks and run them on a thread
        while(networks.size() != numCreatures)
        {
            //create new creature with a survived creature
            Brainstorm::FeedForward newC(networks[(networks.size() % (int)(numCreatures*survivalRate))-1]);

            //randomize network
            newC.Randomize();

            //allocate space for an error
            errorArray.push_back(0);

            std::thread networkThread(RunAndCalc,newC,this->Inputs,this->ExcpectedOutputs,std::ref(errorArray),networks.size());
            networkThread.detach();
            
            networks.push_back(newC);

        }
        
    }
}
