#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat GetFrame(const int screenx, const int screeny, const int width, const int height) {
    cv::Mat src;
    HWND hwnd = GetDesktopWindow();

    // Get handles to a device context (DC)
    const HDC hwindowDC           = GetDC(hwnd);
    const HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // Create a mat object
    src.create(height, width, CV_8UC4);

    const HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);

    // Create a bitmap image
    BITMAPINFOHEADER bi;

    bi.biSize          = sizeof(BITMAPINFOHEADER);  // Size of bitmap header
    bi.biWidth         = width;                     // Bitmap width
    bi.biHeight        = -height;                   // The negative sign makes it not upside down
    bi.biPlanes        = 1;                         // Keep RGB color
    bi.biBitCount      = 32;                        // Number of pixels to represent 1 pixel in the image
    bi.biCompression   = BI_RGB;                    // Compression method
    bi.biSizeImage     = 0;                         // Amount of bits for the image data
    bi.biXPelsPerMeter = 0;                         // Resolution per horizontal direction
    bi.biYPelsPerMeter = 0;                         // Resolution per vertical direction
    bi.biClrUsed       = 0;                         // Number of color indexes
    bi.biClrImportant  = 0;                         // Number of indexes that considered important for displaying

    // Use the previously created device context with bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // Copy from the window DC to the bitmap DC
    StretchBlt(
        hwindowCompatibleDC,
        0, 0,
        width, height,
        hwindowDC,
        screenx, screeny,
        width, height,
        SRCCOPY
    );

    // Copy from hwindowCompatibleDC to hbwindow
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    return src;
}

int main() {
    // Define scale, height, and width
    const unsigned int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    const unsigned int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    const unsigned int width   = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    const   signed int height  = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // Create a video writer object
    cv::VideoWriter video(
        "output.mp4",
        cv::VideoWriter::fourcc('X', '2', '6', '4'),
        10,
        cv::Size(width, height)
    );

    // Check if video writer opened successfully
    if (!video.isOpened()) {
        std::cout << "Could not open video file" << std::endl;
        return -1;
    }

    // Iterate over a series of images and write them to the video
    while (true) {
        // Get screenshot
        cv::Mat frame = GetFrame(screenx, screeny, width, height);

        // Write the frame to the video
        video.write(frame);

        //Make the frame smaller in size
        cv::Mat resizedFrame;
        cv::resize(frame, resizedFrame, cv::Size(640, 480));

        cv::imshow("Frame", resizedFrame);
        if (cv::waitKey(1) == 'q')
            break;
    }

    // Release the video writer and close the output file
    video.release();

    return 0;
}
