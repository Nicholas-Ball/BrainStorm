#include "brainstorm.hpp"

namespace Brainstorm {
    enum TrainingType {NatrualSelection,BackPropagation};

    class Training{
        private:
            std::vector<std::vector<double>> Inputs;
            std::vector<std::vector<double>> ExcpectedOutputs;
            bool Verbose = false;
            int NumCreatures = 100;
            double SurvivalRate = 0.25;
            int Epoches = 1000;

            Brainstorm::FeedForward NatrualSelectionFF(Brainstorm::FeedForward network); // natural selection for feed Forward networks
            Brainstorm::FeedForward BackPropagationFF(Brainstorm::FeedForward network); // backpropagtion for feed Forward networks
        public:
            Brainstorm::FeedForward Train(Brainstorm::FeedForward network,TrainingType t); //Train Feed forward network with specified type
            
            void AddTrainingData(std::vector<double> input,std::vector<double> expectedOutput); //add training data to be used for training

            //natural selection set data
            void SetSurvivalRate(double rate);
            void SetNumberOfCreatures(int num);
            void SetEpoches(int epoches);

            //turn on / off verbose
            void SetVerbose(bool v);

    };
};