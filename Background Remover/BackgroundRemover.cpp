/*
* Image background remover
* Removes the background of a given image and replaces it with a different one
*/

#include <opencv2/opencv.hpp>
#include <iostream>

bool CheckFile(cv::String filename)
{
	size_t dotIndex = filename.find_last_of('.');

	if (dotIndex != std::string::npos) {      // If dotIndex exsist
		std::string extension = filename.substr(dotIndex + 1, sizeof(filename) - 1);

		if (extension == "png" || extension == "bmp" || extension == "jpg" || extension == "jpeg")
			return true;
		else
			return false;
	}

	return false;
}

cv::String GetImageName()
{
	std::string dir = "D:/Mihan/c++/Opencv2/Opencv2";

	std::vector<cv::String> filenames;
	cv::glob(dir, filenames, false);

	if (filenames.empty())
		std::cerr << "Error: invalid dirrectory path or dirrectory does not exsist" << std::endl;

	for (const auto& filename : filenames){
		// Check if file extension ends in png or jpg or bmp before creating it as a choice
		if (CheckFile(filename)) {
			// Extract the image name
			std::string name = " ";
			size_t lastSlashIndex = filename.find_last_of('\\');

			if (lastSlashIndex != std::string::npos)      // If slashed are there
				name = filename.substr(lastSlashIndex + 1, sizeof(filename) - 1);

			std::cout << name << std::endl;
		}
	}

	std::string choice;
	std::cout << "Enter image name of background:";
	std::cin >> choice;

	cv::String ch = choice;
	return ch;
}

int main()
{
	// Load the background, frame and camera
	cv::Mat background = cv::imread(GetImageName());
	cv::VideoCapture camera(0);
	cv::Mat frame;

	// Check if background is loaded properly
	if (background.empty()) {
		std::cerr << "Error: Unable to find background \n";
		return -1;
	}

	// Change the size of background
	camera >> frame;

	const int width  = frame.cols;
	const int height = frame.rows;

	cv::resize(background, background, cv::Size(width, height));

	// Create the background substractor
	cv::Ptr<cv::BackgroundSubtractor> bgSubstractor = cv::createBackgroundSubtractorMOG2();

	// Start recording
	while (true) {
		camera >> frame;

		// Apply background substraction to obtain the foreground mask
		cv::Mat foregroundMask;
		bgSubstractor->apply(frame, foregroundMask);

		// Invert the mask to create a mask for the background
		cv::Mat backgroundMask = ~foregroundMask;

		// Create the background and foreground images
		cv::Mat backgroundImg;
		background.copyTo(backgroundImg, backgroundMask);

		cv::Mat foregroundImg;
		frame.copyTo(foregroundImg, foregroundMask);

		// Combine the background and foreground images
		double transparency = 0.5;

		cv::addWeighted(
			backgroundImg,       // Input 1 
			transparency,        // Alpha      - Adjusts backgroundImg contrast
			foregroundImg,       // Input 2
			1.0 - transparency,  // Beta       - Adjust foregroundImg contrast
			0.0,                 // Gamma      - Adjust brightness
			frame                // Output
		);

		cv::imshow("Camera", frame);

		int key = cv::waitKey(30);

		if (key == 13) {  // If ENTER pressed take a photo
			static int photos = 0;
			photos++;
			cv::imwrite("photo" + std::to_string(photos) + ".png", frame);
		}
		else if (key == 27) // If P pressed
			break;
	}

	camera.release();
	cv::destroyAllWindows();

	return 0;
}