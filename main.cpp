#include "include/Brainstorm/brainstorm.hpp"

int main() {
	Brainstorm::FeedForward ff;
	ff.Generate({2,3,2});
	ff.Run({5,4,2});

	std::cout<<ff.GetOutput()[0]<<std::endl;
} 