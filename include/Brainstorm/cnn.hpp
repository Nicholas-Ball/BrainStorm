#include "brainstorm.hpp"
#include "../img/img.hpp"

namespace Brainstorm {
	class CNN
	{
		private:
			std::vector<std::vector<std::vector<double>>> Scans; //convolutional scan patterns
			Brainstorm::FeedForward FFNetwork; //feedforwad network
			int Pool = 2; //number of times the network preforms conolution and pools the image
		public:
			void Run(Img image); //run cnn network

			std::vector<double> GetOuput(); //get output of network

			void Generate(std::vector<int> matrix); //generate network and pass matrix for feed forward network

			void AddScan(std::vector<std::vector<double>> scan);
	};

};