#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Neuron{
    public:
        double output;
        vector<double> weights;
};

class NeuralNetwork{
    public:
        NeuralNetwork(){
            for (int i = 0; i < 2; i++)
                inputLayer.push_back(Neuron());
            
            for (int i = 0; i < 2; i++){
                hiddenLayer.push_back(Neuron());

                for (int j = 0; j < 2; j++)
                    hiddenLayer[i].weights.push_back(0.5);
            }

            outputLayer.push_back(Neuron());

            for(int i = 0; i < 2; i++)
                outputLayer[0].weights.push_back(-1.0);
            
        }

        double sigmoid(double x){
            return 1.0 / (1.0 + exp(-x));
        }

        double feedForward(double input1, double input2){
            inputLayer[0].output = input1;
            inputLayer[1].output = input2;

            for(int i = 0; i < 2; i++){
                double sum = 0.0;

                for(int j = 0; j < 2; j++)
                    sum += inputLayer[j].output *  hiddenLayer[i].weights[j];
                
                outputLayer[0].output = sigmoid(sum);
                return outputLayer[0].output;
            }
        }

    private:
        vector<Neuron> inputLayer;
        vector<Neuron> hiddenLayer;
        vector<Neuron> outputLayer;
};

int main(){
    NeuralNetwork neuralNetwork;

    cout << "Neural Network for AND gate" << endl;

    double inputs[4][2] = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    double expectedOutputs[4] = {0.0, 0.0, 0.0, 1.0};

    for (int i = 0; i < 4; i++){
        double output = neuralNetwork.feedForward(inputs[i][0], inputs[i][1]);
        int result = (output >= 0.7) ? 1: 0; 
        cout << inputs[i][0] << " AND " << inputs[i][1] << " = " << result << "\n";
    }
}