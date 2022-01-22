#include <fstream>
#include "include/Brainstorm/training.hpp"

int main() {
	Brainstorm::FeedForward ff;
	ff.SetType(Brainstorm::Types::RELU);
	ff.Generate({2,3,2});
	ff.Run({7,2,1});

	std::cout<<ff.GetOutput()[0]<<std::endl;

	Brainstorm::Training t;

	t.AddTrainingData({3,2,5},{3,2});
	t.AddTrainingData({7,2,1},{3,2});

	t.SetNumberOfCreatures(100);
	t.SetEpoches(1000);
	t.SetSurvivalRate(0.25);
	t.SetVerbose(true);

	ff = t.Train(ff,Brainstorm::TrainingType::NatrualSelection);

	ff.Run({7,2,1});

	std::cout<<ff.GetOutput()[0]<<std::endl;
	std::cout<<ff.GetOutput()[1]<<std::endl;

	std::ofstream file("brain.json");
	
	auto j = ff.Save();

	file << j;

	file.close();

	std::ifstream ifile("brain.json");
	j = nlohmann::json::parse(ifile);
	Brainstorm::FeedForward test;
	test.Load(j);

	test.Run({7,2,1});

	std::cout<<test.GetOutput()[0]<<std::endl;
	std::cout<<test.GetOutput()[1]<<std::endl;

	
} 