#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <stdlib.h>

using namespace std;

double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

double feedforward(double x1, double x2, vector<double>& hidden_weights, vector<double>& output_weights, double hidden_bias, double output_bias) {
	double hidden_output1 = sigmoid(x1 * hidden_weights[0] + x2 * hidden_weights[2] + hidden_bias);
	double hidden_output2 = sigmoid(x1 * hidden_weights[1] + x2 * hidden_weights[3] + hidden_bias);
	double output = sigmoid(hidden_output1 * output_weights[0] + hidden_output2 * output_weights[1] + output_bias);

	return output;
}

void backpropagation(double x1, double x2, double expected_output, vector<double>& hidden_weights, vector<double>& output_weights, double& hidden_bias, double& output_bias, double learning_rate) {
	double hidden_output1 = sigmoid(x1 * hidden_weights[0] + x2 * hidden_weights[2] + hidden_bias);
	double hidden_output2 = sigmoid(x1 * hidden_weights[1] + x2 * hidden_weights[3] + hidden_bias);
	double output = sigmoid(hidden_output1 * output_weights[0] + hidden_output2 * output_weights[1] + output_bias);

	double error = expected_output - output;

	output_weights[0] += learning_rate * error * hidden_output1;
	output_weights[1] += learning_rate * error * hidden_output2;
	hidden_weights[0] += learning_rate * error * output_weights[0] * hidden_output1 * (1 - hidden_output1) * x1;
	hidden_weights[1] += learning_rate * error * output_weights[1] * hidden_output2 * (1 - hidden_output2) * x2;
	hidden_weights[2] += learning_rate * error * output_weights[0] * hidden_output1 * (1 - hidden_output1) * x1;
	hidden_weights[3] += learning_rate * error * output_weights[1] * hidden_output2 * (1 - hidden_output2) * x2;
}

void train(vector<double>& hidden_weights, vector<double>& output_weights, double& hidden_bias, double& output_bias, double learning_rate) {
	vector<vector<double>> input_data = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
	vector<double> expected_outputs = { 0, 0, 0, 1 };

	for (int epoch = 0; epoch < 20000; epoch++) {
		int random_index = rand() % input_data.size();
		double x1 = input_data[random_index][0];
		double x2 = input_data[random_index][1];

		double expected_output = expected_outputs[random_index];;
		backpropagation(x1, x2, expected_output, hidden_weights, output_weights, hidden_bias, output_bias, learning_rate);

		if (epoch % 1000 == 0) cout << "Epoch " << epoch << endl;
	}
}

int main() {
	vector<double> hidden_weights = { 0.5, 0.5, 0.5, 0.5 };
	vector<double> output_weights = { 0.5, 0.5 };
	double hidden_bias = 0.5;
	double output_bias = 0.5;

	srand(time(NULL));
	train(hidden_weights, output_weights, hidden_bias, output_bias, 0.1);

	system("cls");

	vector<vector<double>> inputs = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
	vector<double> outputs = { 0, 0, 0, 1 };

	for (int i = 0; i <= 4; i++) {
		double output = feedforward(inputs[i][0], inputs[i][1], hidden_weights, output_weights, hidden_bias, output_bias);
		cout << inputs[i][0] << " AND " << inputs[i][1] << " = " << round(output) << endl;
	}
};