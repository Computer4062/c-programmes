#include <fann.h>
#include <fann_cpp.h>
#include <floatfann.h>
#include <iostream>

using namespace std;

int main() {
	FANN::neural_net nn;
	fann_type input[2];

	const float desired_error = 0.00001;
	const unsigned int max_epochs = 500000;
	const unsigned int epochs_between_reports = 1000;
	const unsigned int layers_count = 3;
	const unsigned int layers[layers_count] = { 2, 2, 1 };

	cout << "Enter the first value here: "; cin >> input[0];
	cout << "Enter the second value here: "; cin >> input[1];

	nn.create_standard_array(layers_count, layers);

	//nn.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
	//nn.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
	
	nn.save("test.train");

	nn.train_on_file("test.train", max_epochs, epochs_between_reports, desired_error);

	fann_type *output = nn.run(input);

	cout << "(" << input[0] << "," << input[1] << ")" << "->" << output[0];
}
