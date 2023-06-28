#include <iostream> 
#include <cmath>
#include <vector>
#include <random>

using namespace std;

class Neuron{
	public:
		double output;
		vector<double> weights;	
};

class NeuralNetwork{
	private:
		vector<Neuron> inputLayer;
		vector<Neuron> hiddenLayer;
		vector<Neuron> outputLayer;
		vector<double> weights;

		double bias;
		double learningRate;

	public: 
		NeuralNetwork();

		double Sigmoid(double sum); //activation function
		double DSigmoid(double sum);
		double FeedForward(double input1, double input2); //propegating through the elements

		void Train(double target, double input1, double input2); //train the model
};

NeuralNetwork::NeuralNetwork(){
	bias = 1.0;
	learningRate = 0.01;

	//initialize weights
	weights.push_back(0.25);
	weights.push_back(0.31);
	weights.push_back(0.55);
	weights.push_back(0.043);

	//pushback input layer neurons
	for(int i = 0; i < 2; i++)
		inputLayer.push_back(Neuron());

	//initialize hidden layer and add weights
	int count = -1;
	for(int i = 0; i < 2; i++){
		hiddenLayer.push_back(Neuron());

		for(int j = 0; j < 2; j++){
			count += 1; //count for extracting weight from weights
			hiddenLayer[i].weights.push_back(weights[count]);
		}
	}

	//initialize output layer
	outputLayer.push_back(Neuron());
};

double NeuralNetwork::FeedForward(double input1, double input2){
	inputLayer[0].output = input1;
	inputLayer[1].output = input2;

	for(int i = 0; i < 2; i++){
		double sum = 0.0;

		for(int j = 0; j < 2; j++)
			sum += (inputLayer[i].output * hiddenLayer[i].weights[j]) + bias;
		
		outputLayer[0].output = Sigmoid(sum);
	}

	return outputLayer[0].output;
}

double NeuralNetwork::Sigmoid(double sum){
	return 1.0 / (1.0 + exp(-sum));
}

double NeuralNetwork::DSigmoid(double sum){
	return Sigmoid(sum) * (1 - Sigmoid(sum)); //sigmoid for calculating error
}

void NeuralNetwork::Train(double input1, double input2, double target){
	double prediction = FeedForward(input1, input2);

	double derror_dprediction = 2 * (prediction - target);  //calculate the difference in error
	double dprediction_dlayer = DSigmoid(prediction); //difference in prediction
	double dlayer_dbias = 1; //make layer bias 1
	vector<double> dlayer_dweights;

	for(int i = 0; i < 2; i++){
		//hiddenLayer[0 or 1]
		for(int j = 0; j < 2; j++){
			//weights[0 or 1] in hiddenLayer[0 or 1]
			double weights = hiddenLayer[i].weights[j];
			dlayer_dweights.push_back(weights); //push weight to dlayer_dweights
		}
	}

	//update bias
	double derror_dbias = derror_dprediction * dprediction_dlayer;
	for(int i = 0; i < 4; i++)
		derror_dbias *= dlayer_dweights[i];

	//update weights
	double derror_dweigths = derror_dprediction * dprediction_dlayer;
	for(int i = 0; i < 4; i++)
		derror_dweigths *= dlayer_dweights[i];

	//update the bias and the weights
	bias = bias - (derror_dbias * learningRate);
	
	//update the weights
	for(int i = 0; i < 4; i++)
		weights[i] = weights[i] - (derror_dweigths * learningRate);
}


int main(){
	double inputs[4][2] = {{0.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {1.0, 0.0}};
	double outputs[4] = {0.0, 0.0, 1.0, 0.0};

	NeuralNetwork neuralNetwork;
	int count = 0;

	while(count < 20000){
		for(int i = 0; i < 4; i++)
			neuralNetwork.Train(inputs[i][0], inputs[i][1], outputs[i]); //train once

		if(count % 100 == 0)
			cout << "Epoch: " << count << endl; //print epoch every 100 counts

		count += 1;
	}

	while(true){
		double input1, input2;
		cin >> input1;
		cin >> input2;

		if(input1 == 0.2 || input2 == 0.2)
			break;

		double prediction = neuralNetwork.FeedForward(input1, input2);
		cout << prediction << endl;
	}

	return 0;
}