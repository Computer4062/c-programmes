#include "Objects.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    Objects object;
    int start, delay;
	string filename;

    while (true){
		cout << "Filename: ";
		cin >> filename;

        cout << "speed: ";
        cin >> delay;

        wstring content = object.ReadFile(filename);
        object.StartTyper(content, delay);
    }
}