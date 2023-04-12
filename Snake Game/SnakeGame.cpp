#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirrection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirrection dir;

void title(){
	cout <<	"  _____                   _             _   ____              _        \n";  
	cout << "|_   _|__ _ __ _ __ ___ (_)_ __   __ _| | / ___| _ __   __ _| | _____  \n";
	cout << "  | |( _ ( '__| '_ ` _ (| | '_ ) ( _` | | (___ )| '_ ) ( _` | |( ( _  \n";
	cout << "  | |  __) |  | | | | | | | | | | (_| | |  ___) | | | | (_| |   <  __) \n";
	cout << "  |_|(___|_|  |_| |_| |_|_|_| |_|(__,_|_| |____)|_| |_|(__,_|_|(_(___| \n";                                                           
}

void Setup(){
    gameOver = false;
    dir = STOP;

    x = width / 2;
    y = height / 2;

    fruitX = rand() % width;
    fruitY = rand() % height;

    score = 0;
}

void Draw(){
    system("cls");
    for(int i = 0; i < width + 2; i++){
        cout << "#";
    }

    cout << endl;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(j == 0) cout << "#"; 

            if(i == y && j == x) cout << "0";
            else if(i == fruitY && j == fruitX) cout << "F";
            else{

                bool print = false;
                for(int k = 0; k < nTail; k++){

                    if(tailX[k] == j && tailY[k] == i){
                        cout << "o";
                        print = true;
                    }
                }
                if(!print) cout << " ";
            }
            if(j == width-1) cout << "#";
        }
        cout << endl;
    }

    for(int i = 0; i < width + 2; i++){
        cout << "#";
    }

    cout << endl;
    cout << "Score: " << score << endl;
}

void Input(){
    if(_kbhit()){
        switch(_getch()){
            case 'a':
                dir = LEFT;
                break; 

            case 'w':
                dir = UP;
                break;
            
            case 's':
                dir = DOWN;
                break;

            case 'd':
                dir = RIGHT;
                break;
            
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void Logic(){
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    
    for(int i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;

    }

    switch(dir){
        case STOP:
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    if(x > width || x < 0 ||  y > height || y < 0) gameOver = true;
    
    for(int i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y) gameOver = true;
    }

    if(x == fruitX && y == fruitY){
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        
        nTail++;
    }
}

int main(){
    int c, speed;
	title();
    cout << "select the speed here: \n";
    cout << "level 1 \n";
    cout << "level 2 \n";
    cout << "level 3 \n";
    cout << "level 4 \n";
    cout << "level 5 \n";

    cout << ">>"; cin >> c;

    switch(c){
        case 1: speed = 100; break;
        case 2: speed = 80; break;
        case 3: speed = 60; break;
        case 4: speed = 20; break;
        case 5: speed = 10; break;
        default: break;
    }

    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();

        Sleep(speed);
    }
}