#include "emulator.h"
#include <string>
#include <iostream>
#include <conio.h>
#include "SDL.h"

using namespace std;

string games[22] = {
    "15PUZZLE", "BLINKY", "BRIX", "CONNECT4", "GUESS",
    "HIDDEN", "INVADERS", "KALEID", "MAZE", "MERLIN", 
    "MISSILE", "PONG", "PONG2", "PUZZLE", "SYZYGY", 
    "TANK", "TETRIS", "TICTAC", "UFO", "VBRIX",
    "VERS", "WIPEOFF"
};

bool recheck = false;

int main(int argc, char* argv[]) {
    
    cout << "15PUZZLE     BLINKY       BRIX       CONNECT4     GUESS" << endl;
    cout << "HIDDEN       INVADERS     KALEID     MAZE         MERLIN" << endl;
    cout << "MISSILE      PONG         PONG2      PUZZLE       SYZYGY" << endl;
    cout << "TANK         TETRIS       TICTAC     UFO          VBRIX" << endl;
    cout << "VERS         WIPEOFF" << endl; 
    
    Emulator emulator;
    int value = 0;

    while (true) {
        if(recheck == true){
            system("cls");
            
            cout << "15PUZZLE     BLINKY       BRIX       CONNECT4     GUESS" << endl;
            cout << "HIDDEN       INVADERS     KALEID     MAZE         MERLIN" << endl;
            cout << "MISSILE      PONG         PONG2      PUZZLE       SYZYGY" << endl;
            cout << "TANK         TETRIS       TICTAC     UFO          VBRIX" << endl;
            cout << "VERS         WIPEOFF" << endl; 

            recheck = false;
        }

        cout << "value: " << games[value] << "\r";
        if (_kbhit()) {
            int key = _getch();

            if(key == 13){
                string selected = "games/" + games[value];

                recheck = true;
                SDL_Init(SDL_INIT_EVERYTHING);
                if(!emulator.CHIP8emulator(selected))
                    cout << "Something went wrong" << endl;
            }

            if (key == 224) {
                key = _getch();

                if (key == 72) {
                    if(value == 0 || value != 21) value++;
                    else if(value == 21) value--;
                }

                if(key == 80){
                    if(value == 21 || value != 0) value--;
                    else if(value == 0) value++;
                }
            }
        }
    }

    return 0;
}