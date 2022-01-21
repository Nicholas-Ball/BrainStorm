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

	t.Train(ff,Brainstorm::TrainingType::NatrualSelection,1000);

	std::cout<<ff.GetOutput()[0]<<std::endl;
} 