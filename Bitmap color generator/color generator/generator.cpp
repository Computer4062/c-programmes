#include <iostream> 
#include <fstream>
#include <sstream> 
#include <string>
#include <windows.h>

#define FILE_NAME "color.bmp"
#define WIDTH     800
#define HEIGHT    600

#pragma pack(push, 1)

struct BitmapFileHeader {
	char         signature[2]; // Image format
	unsigned int fileSize;     // Image size
	unsigned int reserved;     // Extra bits
	unsigned int dataOffset;   // size from beginning of file to the pixel data
};

struct BitmapInfoHeader {
	unsigned int   headerSize;           //Size of header
	int            width;                // Image width
	int            height;               // Image height
	unsigned short colorPlanes;          // Number of clors
	unsigned short bitsPerPixel;         // Bits required to make 1 pixel
	unsigned int   compressionMethod;    // Compression method, 0 for none
	unsigned int   imageDataSize;        // Size of image
	int            horizontalResolution; // Amount of bits horizontally
	int            verticleResolution;   // Amount of bits vertically
	unsigned int   colorsInPalette;       // Colors in the image
	unsigned int   importantColors;      // Important colors
};

#pragma pack(pop)

struct Color {
	unsigned char r, g, b;
};

void GenerateImage(const std::string& filename, const Color& color)
{
		std::ofstream file(FILE_NAME, std::ios::binary);

		if(!file){
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
		infoHeader.horizontalResolution = 2835;
		infoHeader.verticleResolution = 2835;
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

int main()
{	
	std::cout << "Enter values in RGB format: ";
	std::string values;
	std::getline(std::cin, values);

	std::string red   = values.substr(0, 3);
	std::string green = values.substr(4, 3);
	std::string blue  = values.substr(8, 3);

	unsigned char r = static_cast<unsigned char>(std::stoi(red));
	unsigned char g = static_cast<unsigned char>(std::stoi(green));
	unsigned char b = static_cast<unsigned char>(std::stoi(blue));

	Color customColor = {r, g, b};
	GenerateImage(FILE_NAME, customColor);

	return 0;
}