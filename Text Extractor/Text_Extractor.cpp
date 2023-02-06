#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;

void textGenerate() {
	cout << "==================================================================================================\n";
	cout << "======= |=====  ||      ||  =======   |=====| |===== |===== ||       || | |====   |===== |===|\n" ;
	cout << "  =     |        ||   ||       =      |     | |    | |    |  ||     ||  | |    =  |      |   |\n" ;   
	cout << "  =     |-----     |||         =      |=====| |====| |    |   ||   ||   | |    =  |----  |===|\n" ;
	cout << "  =     |         ||  ||       =      |       |||    |    |   ||   ||   | |    =  |      |||  \n" ;
	cout << "  =     |=====   ||    ||      =      |       |  ||  |====|   |||||||   | |====   |====  | || \n" ;
	cout << "==================================================================================================\n";
}

class ReadText {
public:
	char getText(cv::Mat image);
	void Camera();
	void ReadInstructions();
	void Image();
};

char ReadText::getText(cv::Mat image) {
	char* outtext;
	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();

	if (api->Init(NULL, "eng")) {
		cout << stderr << "Cannot intialize tesseract";
		exit(1);
	}

	api->SetImage(image.data, image.cols, image.rows, 4, image.cols * 4);

	outtext = api->GetUTF8Text();

	cout << "result: \n" << outtext;

	api->End();
	delete api;
	delete[] outtext;

	return 'a';
}

void ReadText::Camera() {
	system("cls");
	system("color C");
	cv::VideoCapture Cap(0);
	cv::Mat img;

	while (true) {
		Cap.read(img);
		cv::imshow("image", img);

		cv::waitKey(1);
	}
}

void ReadText::ReadInstructions() {
	system("cls");
	system("color a");
	char ch;
	cout << "======================================\n";
	cout << " \t Instructions:\t\n ";
	cout << "======================================\n";

	cout << "This is a live writtern text to text converter so write your text and show your text to the camera and it would print the text to the console \n";

	cout << "Enter a letter on your keyboard to go back\n";
	cout << " >>"; cin >> ch;
}

void ReadText::Image() {
	system("cls");
	system("color B");
	char location[100];
	char* outtext;
	cout << "Enter the file location of the image: "; cin >> location;

	Pix* img = pixRead(location);

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) {
		cout << stderr << "Could not intialize tesseract \n";
		exit(1);
	}

	api->SetImage(img);

	outtext = api->GetUTF8Text();
	cout << "result: \n" << outtext;

	api->End();
	delete api;
	delete[]outtext;
	pixDestroy(&img);

	cout << "Press any key of your keyboard to continue: ";
	cin >> location;
}

int main() {
	int ch;
	ReadText readtext;
	do {
		system("cls");
		system("color F");
		textGenerate();

		cout << "\n  Select an option: \n";
		cout << " ----------------------- \n";
		cout << "1) Live text converting \n";
		cout << "2) Convert Text \n";
		cout << "3) Read instructions \n";
		cout << "4) Exit \n";
		cout << ">>"; cin >> ch;

		switch (ch) {
		case 1:
			readtext.Camera();
			break;
		case 2:
			readtext.Image();
			break;
		case 3:
			readtext.ReadInstructions();
			break;
		case 4:
			exit(1);
			break;
		default:
			break;
		}
	} while (ch != 4);
}