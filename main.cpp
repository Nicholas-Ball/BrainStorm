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
	ff.Generate({5,5,5,5,5,5,5,1},Brainstorm::Types::SIGMOID);
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
	tr.SetAutomated(true);

	std::cout<<"test1"<<std::endl;
	//add dice
	for(int i = 1; i != 5;i++)
	{
		Img dice;
		dice.Load("dice"+std::to_string(i)+".jpg");
		cnn.Run(dice);
		auto out = cnn.GetOutput();
		std::cout<<out.size()<<std::endl;
		tr.AddTrainingData(out,{1});
	}
	
	std::cout<<"test2"<<std::endl;

	//add not dice
	for(int i = 1; i != 5;i++)
	{
		auto img = Img("notdice"+std::to_string(i)+".jpg");
		cnn.Run(img);
		tr.AddTrainingData(cnn.GetOutput(),{0});
	}
	
	std::cout<<"test3"<<std::endl;

	//train
	tr.SetVerbose(true);
	ff = tr.Train(ff,Brainstorm::Training::TrainingType::BackPropagation);

	

	auto j = ff.Save();
	std::ofstream file("brain.json"); 
	file << j;
	file.close();
	/**/

	cnn.Run(Img("dice1.jpg"));
	ff.Run(cnn.GetOutput());
	std::cout<<"Is Dice: "<<"is: "<<ff.GetOutput()[0]<<std::endl;

	auto t1 = cnn.GetOutput();

	cnn.Run(Img("notdice1.jpg"));
	ff.Run(cnn.GetOutput());
	std::cout<<"Is Not Dice: "<<"is: "<<ff.GetOutput()[0]<<std::endl;

	auto t2 = cnn.GetOutput();


} 