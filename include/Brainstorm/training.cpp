#include "training.hpp"
#include <functional>
#include <map>
#include <bits/stdc++.h>

int count = 0;

void sort(std::vector<Brainstorm::FeedForward> *arr)
{
	//return arrays of size 1
	if(arr->size() <= 1) return;

	std::vector<Brainstorm::FeedForward> arr1;
	std::vector<Brainstorm::FeedForward> arr2;

	int size = (int)(arr->size()/2);
	//split array
	for(int i = 0; i != arr->size();i++)
	{
		if(i < size)
		{
			arr1.push_back(arr->at(i));
		}
		else
		{
			arr2.push_back(arr->at(i));
		}
	}

	//sort split arrays
	sort(&arr1);
	sort(&arr2);

	//blank for sorting
	arr = {};

	std::vector<Brainstorm::FeedForward> temp;

	//sort
	while(arr1.size() != 0 && arr2.size() != 0)
	{
		if(arr1[0].error < arr2[0].error)
		{
			temp.push_back(arr1[0]);
			arr1.erase(arr1.begin());
		} else
		{
			temp.push_back(arr2[0]);
			arr2.erase(arr2.begin());
		}
		arr = &temp;
	}


	if(arr1.size() == 1)
	{
		temp.push_back(arr1[0]);
	}else if(arr2.size() == 1)
	{
		temp.push_back(arr2[0]);
	}

	arr = &temp;

}


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
void static RunAndCalc(Brainstorm::FeedForward *network,std::vector<std::vector<double>> *input,std::vector<std::vector<double>> *expectedOutput)
{
    network->error = 0;

		std::cout<<network->error<<std::endl;

		std::cout<<input->size()<<std::endl;
    //run through input data and add calculate error
    for(int i = 0;i != input->size();i++)
    {

			std::cout<<"Network size: "<<network->GetNetwork()->size()<<std::endl;
        //run network
        network->Run(input->at(i));

        //get network output
        auto out = network->GetOutput();	

        //calculate error and add it to error total
        for(int o = 0; o != out.size();o++)
        {
            network->error += (expectedOutput->at(i)[o] - out[o])*(expectedOutput->at(i)[o] - out[o]);
        }

			std::cout<<"Test3"<<std::endl;	

    }

		std::cout<<"thread finished"<<std::endl;
}

//feed forward natural selection training
void Brainstorm::Training::NatrualSelectionFF(Brainstorm::FeedForward network,int Epoches)
{
    //array of creature networks
    std::vector<Brainstorm::FeedForward> networks = {network};

    //number of creatures to have at one time
    int numCreatures = 100;

		//set the natural selection survival rate
    double survivalRate = 0.25;

    //run through Epoches
    for(int e = 0; e != Epoches;e++)
    {
			//array of threads to later rejoin them
			std::vector<std::thread> threads;
        //generate networks and run them on a thread
        while(networks.size() != numCreatures)
        {
            //create new creature with a survived creature
            Brainstorm::FeedForward newC(networks[networks.size()-1 % ((int)(numCreatures * survivalRate)-1)]);

            //randomize network
            newC.Randomize();

            threads.push_back(std::thread(RunAndCalc,&newC,&this->Inputs,&this->ExcpectedOutputs));

            networks.push_back(newC);

        }

				//rejoin threads
				for(int t = 0; t != threads.size();t++)
				{
					threads[t].join();
				}
				//sort error smallest to biggest
				sort(&networks);

				//make data to remove creatures that didn't survive
				std::vector<Brainstorm::FeedForward> temp;

				//remove all creatures that didn't survive
				for(int n = 0; n != 25;n++)
				{
					std::cout<<n<<": "<<networks[n].error<<std::endl;
				}

				
				//set data of survived creatures
				networks = temp;
    }

		network = networks[0];
}
