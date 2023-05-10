#include <iostream>
#include <fstream>

using namespace std;

void compressRLE(const string& inputFile, const string& outputFile) {
	ifstream in(inputFile, ios::binary);
	ofstream out(outputFile, ios::binary);

	char prevChar, currChar;
	int count;
	in.get(prevChar);
	count = 1;

	while (in.get(currChar)) {
		if (currChar == prevChar) {
			count++;
		} else {
			out.write(reinterpret_cast<const char*>(&count), sizeof(int));
			out.put(prevChar);
			prevChar = currChar;
			count = 1;
		}
	}
	out.write(reinterpret_cast<const char*>(&count), sizeof(int));
	out.put(prevChar);

	in.close();
	out.close();
}

void decompressRLE(const string& inputFile, const string& outputFile) {
	ifstream in(inputFile, ios::binary);
	ofstream out(outputFile, ios::binary);

	char currChar;
	int count;
	while (in.read(reinterpret_cast<char*>(&count), sizeof(int))) {
		in.get(currChar);
		for (int i = 0; i < count; i++) {
			out.put(currChar);
		}
	}

	in.close();
	out.close();
}

int main() {
	compressRLE("input.txt", "output.rle");
	decompressRLE("output.rle", "output.txt");

	return 0;
}