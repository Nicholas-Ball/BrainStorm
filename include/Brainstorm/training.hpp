#include "brainstorm.hpp"

namespace Brainstorm {
    enum TrainingType {NatrualSelection};

    class Training{
        private:
            std::vector<std::vector<double>> Inputs;
            std::vector<std::vector<double>> ExcpectedOutputs;

            void NatrualSelectionFF(Brainstorm::FeedForward network,int Epoches); //natural selection for feed Forward networks
        public:
            void Train(Brainstorm::FeedForward network,TrainingType t,int Epoches); //Train Feed forward network with specified type
            
            void AddTrainingData(std::vector<double> input,std::vector<double> expectedOutput); //add training data to be used for training
    };
};