#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <utility>

using namespace std;

//Define the huffman tree node structure
struct huffmanNode {
	char         character;
	int          frequency;
	huffmanNode* left;
	huffmanNode* right;
};

//Compare function for priority queue
struct Compare {
	bool operator() (huffmanNode* a, huffmanNode* b) {
		return a->frequency > b->frequency;               //Check if it is a huffman leaf
	}
};

//Function to build huffman tree
huffmanNode* BuildHuffmanTree(const unordered_map<char, int>& frequencies) {
	priority_queue<huffmanNode*, vector<huffmanNode*>, Compare> pq;

	for (auto& pair : frequencies) {
		char character = pair.first;     
		int  frequency = pair.second;

		huffmanNode* node = new huffmanNode;

		node->character = character;
		node->frequency = frequency;
		node->left      = nullptr;
		node->right     = nullptr;

		pq.push(node);
	}

	while (pq.size() > 1) {   //While you are not at the top of the huffman tree

	    /* 
		   pq.top() returns the element with the highest priority in the priority queue
		   which is the element with the lowest frequency. pq.pop() removes it by one
		   which reduces the size of the queue by one 
		*/

		huffmanNode* left = pq.top();
		pq.pop();

		/* 
		   The next element with the lowest frequency is added to the right pointer and
		   removed
		*/

		huffmanNode* right = pq.top();
		pq.pop();

		huffmanNode* parent = new huffmanNode;

		parent->character = '\0'; // Internal node marker
		parent->frequency = left->frequency + right->frequency;
		parent->left      = left;
		parent->right     = right;

		pq.push(parent);
	}

	huffmanNode* root = pq.top();
	return root;

	/*
		We only return the bottom nodes in the tree because
		that is all that is needed to know how to continue
		the tree and decompress text
	*/
}

//Function to traverse the huffman tree and generate huffmen codes
void GenerateHuffmanCodes(huffmanNode* root, string code, unordered_map<char, string>& huffmanCodes)
{
	if (nullptr == root->left && nullptr == root->right) {
		huffmanCodes[root->character] = code;
		return;
	}

	GenerateHuffmanCodes(root->left , code + "0", huffmanCodes);
	GenerateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

/*
Compress the input file using huffmen codng and
and write the comrpessed output to the output file
*/

void Compress(const string& inputFile, const string& outputFile) 
{
	//Read the input file and calculate the character frequencies
	ifstream inFile(inputFile);
	unordered_map<char, int> frequencies;

	char c;

	while (inFile.get(c)) {
		//every time that specific character is recognized increase it's number by one
		frequencies[c]++;
	}

	inFile.close();

	//Build the huffman tree 
	huffmanNode* root = BuildHuffmanTree(frequencies);

	//Generate huffman codes
	unordered_map<char, string> huffmanCodes;
	GenerateHuffmanCodes(root, "", huffmanCodes);

	//Write the compressed output to the output file
	fstream outFile(outputFile, ios::binary | ios::out);
	inFile.open(inputFile);         //re-open the input file

	/*
		For each character from the input file, the line retrieves it's corresponding
		HuffmanCodes and store it in the output file

		The for loop iterates over each character in the code string which represents
		the huffman code for the current character

		bit - '0' converters the character '0' or '1' to the corresponding integer value
		0 or 1 and writes to the output file. This right the binary representation of 
		the huffmanCode to the output file.
	*/

	while (inFile.get(c)) {
		const string code = huffmanCodes[c];
		for (char bit : code) {
			outFile << bit - '0';
		}
	}

	inFile.close();
	outFile.close();

	//Write the frequencies into a file
	cout << "Saving frequencies... \n";

	fstream  characters("characters.txt", ios::out);
	fstream  numbers("numbers.txt", ios::out);

	for (const auto& pair : frequencies) {
		char ch = pair.first;
		int  num = pair.second;

		characters << ch << endl;
		numbers << num << endl;

	}

	characters.close();
	numbers.close();

	cout << "File compressed successfully! \n";
}

//Decompress the input file using huffman coding and write the decompressed file to the output file
void Decompress(const string& inputFile, const string& outputFile)
{
	unordered_map<char, int> frequencies;

	//Get frequencies from the numbers.txt  and characters.txt files
	ifstream characters("characters.txt");

	if (!characters.is_open()) {
		cout << "Error: characters.txt cannot be found" << endl;
		return;
	}

	ifstream numbers("numbers.txt");

	if (!numbers.is_open()) {
		cout << "Error: numbers.txt cannot be found" << endl;
		return;
	}

	string chline, numline;
	while (getline(characters, chline) && getline(numbers, numline)) {
		if (!chline.empty() && !numline.empty()) {
			char character = chline[0];
			int frequency = stoi(numline);
			frequencies[character] = frequency;
		}
	}

	characters.close();
	numbers.close();

	//Re-create the huffman tree
	huffmanNode* root = BuildHuffmanTree(frequencies);

	ifstream inFile (inputFile,  ios::binary);
	fstream  outFile(outputFile, ios::out);

	huffmanNode* currentNode = root;

	char bit;
	while (inFile.get(bit)) {
		if ('0' == bit && currentNode->left != nullptr)
			currentNode = currentNode->left;
		else if ('1' == bit && currentNode->right != nullptr)
			currentNode = currentNode->right;

		// Check if both left and right child are nullptr to identify a leaf node
		if (currentNode->left == nullptr && currentNode->right == nullptr) {
			outFile << currentNode->character;
			currentNode = root;
		}
	}

	inFile.close();
	outFile.close();

	cout << "File decompressed successfully!" << endl;
}

int main(int argc, char* argv[])
{
	string flag       = argv[1];                                    //There are 2 flags -c and -d compress and decompress
	string inputFile  = argv[2];                           
	string outputFile = argv[3];

	if ("-c" == flag) Compress(inputFile, outputFile);
	else if ("-d" == flag) Decompress(inputFile, outputFile);
	else cout << "Error: unrecognized flag";
}