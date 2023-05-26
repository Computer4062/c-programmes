#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> // Add this header for std::transform

unsigned int SHF(const std::string& input) {
    unsigned int Init = 1234564352;
    unsigned int Magic = 5674356;
    unsigned int Hash = Init; // Initialize Hash

    for (int i = 0; i < input.length(); i++) {
        Hash = Hash ^ (input[i]); // xor each byte of input
        Hash = Hash * Magic; // multiply by magic number
    }

    return Hash;
}

std::string ToHex(unsigned int input) {
    std::string HexHash;
    std::stringstream hexstream;

    hexstream << std::hex << input;
    HexHash = hexstream.str();

    std::transform(HexHash.begin(), HexHash.end(), HexHash.begin(), ::toupper);
    return HexHash;
}

int main() {
    std::cout << "tested hash: " << ToHex(SHF("testing"));
    return 0; // Add a return statement to main
}
