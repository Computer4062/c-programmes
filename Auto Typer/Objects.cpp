#include "Objects.h"
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

wstring Objects::ReadFile(const string &filename){
    wstring text;
    wifstream file(filename); //load file

    if(file.is_open()){
        wstring line; 
        while(getline(file, line)) //get line by line
            text += line + L'\n'; //go to the next line

        file.close();
    }

    return text;
}

void Objects::StartTyper(const wstring &str, int delay){
    Sleep(2000); //wait 2 seconds
    wstring typedString;

    //press keys
    for(size_t i = 0; i < str.size(); i++){
        typedString += str[i];

        //press a key
        INPUT input;
        input.type = INPUT_KEYBOARD; //take keyboard input
        input.ki.wVk = 0; //no virtual keyboard
        input.ki.wScan = typedString[i]; //key to press
        input.ki.dwFlags = KEYEVENTF_UNICODE; //press character

        SendInput(1, &input, sizeof(INPUT));
        Sleep(delay); //delay between each character
    }

    //keyup
    for(size_t i = 0; i < typedString.size(); i++){
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0;
        input.ki.wScan = typedString[i];
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

        SendInput(1, &input, sizeof(INPUT));
    }
}