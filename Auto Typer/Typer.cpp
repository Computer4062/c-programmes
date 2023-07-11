#include "Objects.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    Objects object;
    int start, delay;

    while (true){
        cout << "speed: ";
        cin >> delay;
        
        const string filename = argv[1];

        const wstring content = object.ReadFile(filename);
        object.StartTyper(content, delay);
    }
}
