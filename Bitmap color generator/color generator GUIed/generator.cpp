#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>

#define FILE_NAME "color.bmp"
#define WIDTH     800
#define HEIGHT    600

#pragma pack(push, 1)

struct BitmapFileHeader {
    char signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int dataOffset;
};

struct BitmapInfoHeader {
    unsigned int headerSize;
    int width;
    int height;
    unsigned short colorPlanes;
    unsigned short bitsPerPixel;
    unsigned int compressionMethod;
    unsigned int imageDataSize;
    int horizontalResolution;
    int verticalResolution;
    unsigned int colorsInPalette;
    unsigned int importantColors;
};

#pragma pack(pop)

struct Color {
    unsigned char r, g, b;
};

void GenerateBitmap(const Color& color) {
    std::ofstream file(FILE_NAME, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to create file: " << FILE_NAME << std::endl;
        return;
    }

    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    fileHeader.signature[0] = 'B';
    fileHeader.signature[1] = 'M';
    fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + WIDTH * HEIGHT * 3;
    fileHeader.reserved = 0;
    fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    infoHeader.headerSize = sizeof(BitmapInfoHeader);
    infoHeader.width = WIDTH;
    infoHeader.height = HEIGHT;
    infoHeader.colorPlanes = 1;
    infoHeader.bitsPerPixel = 24;
    infoHeader.compressionMethod = 0;
    infoHeader.imageDataSize = WIDTH * HEIGHT * 3;
    infoHeader.horizontalResolution = 2835;  // 72 DPI (dots per inch)
    infoHeader.verticalResolution = 2835;    // 72 DPI (dots per inch)
    infoHeader.colorsInPalette = 0;
    infoHeader.importantColors = 0;

    // Write bitmap headers
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));

    // Image data
    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            file.write(reinterpret_cast<const char*>(&color.b), 1);
            file.write(reinterpret_cast<const char*>(&color.g), 1);
            file.write(reinterpret_cast<const char*>(&color.r), 1);
        }

        int padding = (4 - (WIDTH * 3) % 4) % 4;
        if (padding > 0)
            file.write("\0\0\0", padding);
    }

    file.close();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;                  // Details of paint
            HDC hdc = BeginPaint(hwnd, &ps); // Window handle

            HBITMAP hBitmap = (HBITMAP)LoadImageA(
                NULL, 
                FILE_NAME, 
                IMAGE_BITMAP, 
                0, 0, 
                LR_LOADFROMFILE
            );

            HDC hdcMem = CreateCompatibleDC(hdc);          // Create a device context to the window
            HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap); // Get common objects in hdcMem and hBitmap

            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap); // Get objects of hBitmap and put it into bitmap

            HDC hdcBuffer = CreateCompatibleDC(hdc);   // Create a compatible device Context to hdc

            HBITMAP hBitmapBuffer = CreateCompatibleBitmap(
                hdc, 
                bitmap.bmWidth, 
                bitmap.bmHeight
            );

            // Get common objects used in hdcBuffer and hbitmapBuffer
            HBITMAP hBitmapBufferOld = (HBITMAP)SelectObject(hdcBuffer, hBitmapBuffer);

            // Transfer image from loaded bitmap to the windows DC.
            // This enables bitmap to be displayed on the Window Surface.
            BitBlt(
                hdcBuffer, 
                0, 0, 
                bitmap.bmWidth, bitmap.bmHeight, 
                hdcMem, 
                0, 0, 
                SRCCOPY
            );

            BitBlt(
                hdc, 
                0, 0, 
                bitmap.bmWidth, bitmap.bmHeight, 
                hdcBuffer, 
                0, 0, 
                SRCCOPY
            );

            SelectObject(hdcBuffer, hBitmapBufferOld); // Turn original GDI back into a specified DC
            DeleteObject(hBitmapBuffer);
            DeleteDC(hdcBuffer);

            SelectObject(hdcMem, hBitmapOld); // Take original GDI and make it back in to a DC
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);

            EndPaint(hwnd, &ps);
            return 0;
        }   

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {    
std::cout << "Enter values in RGB format: ";
	std::string values;
	std::getline(std::cin, values);

	std::string red   = values.substr(0, 3);
	std::string green = values.substr(4, 3);
	std::string blue  = values.substr(8, 3);

	unsigned char r = static_cast<unsigned char>(std::stoi(red));
	unsigned char g = static_cast<unsigned char>(std::stoi(green));
	unsigned char b = static_cast<unsigned char>(std::stoi(blue));

    Color color = {r, g, b};
    GenerateBitmap(color);

    const char CLASS_NAME[] = "BitmapWindowClass";

    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Bitmap Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIDTH, HEIGHT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (hwnd == NULL) {
        std::cerr << "Failed to create window." << std::endl;
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}