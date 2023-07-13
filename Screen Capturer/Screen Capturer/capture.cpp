#include <Windows.h>
#include <opencv2/opencv.hpp>

int main()
{

	cv::Mat src;
	HWND hwnd = GetDesktopWindow();

	//Get handles to a device context (DC)
	const HDC hwindowDC           = GetDC(hwnd);
	const HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	//Define scale, height and width
	const unsigned int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const unsigned int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
	const unsigned int width   = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const   signed int height  = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	//Create a mat object
	src.create(height, width, CV_8UC4);

	const HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);

	//Create a bitmap image
	BITMAPINFOHEADER bi;

	bi.biSize          = sizeof(BITMAPINFOHEADER);   //Size of bitmap header
	bi.biWidth         = width;                      //Bitmap width
	bi.biHeight        = -height;                    //The negative sign makes it not upside down
	bi.biPlanes        = 1;                          //Keep RGB color
	bi.biBitCount      = 32;                         //Number of pixels to represent 1 pixel in the image
	bi.biCompression   = BI_RGB;                     //Compression method
	bi.biSizeImage     = 0;                          //Amount of bits for the image data
	bi.biXPelsPerMeter = 0;                          //Resolution per horizontal dirrection
	bi.biYPelsPerMeter = 0;                          //Resolution per verticle dirrection
	bi.biClrUsed       = 0;                          //Number of color indexes 
	bi.biClrImportant  = 0;                          //Number of indexes that considered important for displaying

	//Use the previousely created device context with bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);

	//Copy from the window DC to the bitmap DC
	StretchBlt(
		hwindowCompatibleDC,
		0, 0,
		width, height,
		hwindowDC,
		screenx, screeny,
		width, height,
		SRCCOPY
	);

	//Copy from hwindowCompatibleDC to hbwindow
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	//Save img
	cv::imwrite("Screenshot.png", src);
}