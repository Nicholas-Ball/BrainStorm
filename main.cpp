#include <iostream>
#include <fstream>
#include "include/Brainstorm/training.hpp"
#include "include/Brainstorm/cnn.hpp"



int main() {
	/*
	std::ifstream fileR("brain.json");
	auto j = nlohmann::json::parse(fileR);
	Brainstorm::FeedForward ff;
	ff.Load(j);
	fileR.close();
	/**/
	
	/**/
	//make network
	Brainstorm::FeedForward ff;
	ff.Generate({20,1});
	ff.SetType(Brainstorm::Types::SIGMOID);
	/**/
	//initilize cnn
	Brainstorm::CNN cnn;
	
	cnn.AddScan({
		{0,1,0},
		{0,1,0},
		{0,1,0},
	});

	cnn.AddScan({
		{0,0,0},
		{1,1,1},
		{0,0,0},
	});

	cnn.AddScan({
		{0,0,1},
		{0,1,0},
		{1,0,0},
	});
	/**/
	//make training data
	Brainstorm::Training tr;

	tr.SetTrainingRate(0.1);

	//add dice
	for(int i = 1; i != 5;i++)
	{
		cnn.Run(Img("dice"+std::to_string(i)+".jpg"));
		tr.AddTrainingData(cnn.GetOutput(),{1});
	}

	//add not dice
	for(int i = 1; i != 5;i++)
	{
		cnn.Run(Img("notdice"+std::to_string(i)+".jpg"));
		tr.AddTrainingData(cnn.GetOutput(),{0});
	}

	//train
	tr.SetVerbose(true);
	tr.SetEpoches(15);
	ff = tr.Train(ff,Brainstorm::Training::TrainingType::BackPropagation);

	

	auto j = ff.Save();
	std::ofstream file("brain.json"); 
	file << j;
	file.close();
	/**/

	cnn.Run(Img("dice4.jpg"));
	ff.Run(cnn.GetOutput());
	std::cout<<"Dice: "<<ff.GetOutput()[0]<<std::endl;

	auto t1 = cnn.GetOutput();

	cnn.Run(Img("notdice1.jpg"));
	ff.Run(cnn.GetOutput());
	std::cout<<"Not Dice: "<<ff.GetOutput()[0]<<std::endl;

	auto t2 = cnn.GetOutput();

	std::cout<<(t1==t2)<<std::endl;

} 