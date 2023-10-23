#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "game.h"

using namespace std;

Sudoku::Sudoku()
{
    cout << "COMPUTING VALUES IN Sudoku()" << endl;
    // 1
    values.block_1_v[0][0] = generate_number_not(0, 0, 0, 0);
    values.block_1_v[1][0] = generate_number_not(values.block_1_v[0][0], 0, 0, 0);
    values.block_2_v[0][0] = generate_number_not(values.block_1_v[0][0], values.block_1_v[1][0], 0, 0);
    values.block_2_v[1][0] = generate_number_not(values.block_1_v[0][0], values.block_1_v[1][0], values.block_2_v[0][0], 0);

    // 2
    values.block_1_v[0][1] = generate_number_not(values.block_1_v[0][0], values.block_1_v[1][0], 0, 0);
    values.block_3_v[0][0] = generate_number_not(values.block_1_v[0][0], 0, 0, 0);
    values.block_3_v[0][1] = generate_number_not(values.block_1_v[0][0], values.block_1_v[0][1], values.block_3_v[0][0], 0);

    // 3
    values.block_1_v[1][1] = generate_number_not(values.block_1_v[0][0], values.block_1_v[1][0], values.block_1_v[0][1], 0);

    // 4
    values.block_2_v[0][1] = generate_number_not(values.block_1_v[1][1], values.block_2_v[0][0], values.block_2_v[1][0], 0);

    // 5
    values.block_2_v[1][1] = generate_number_not(values.block_2_v[0][0], values.block_2_v[1][0], values.block_2_v[0][1], 0);

    // 6
    values.block_3_v[1][0] = generate_number_not(values.block_3_v[0][0], values.block_1_v[1][1], values.block_1_v[1][0], 0);

    // 7
    values.block_3_v[1][1] = generate_number_not(values.block_3_v[0][0], values.block_3_v[0][1], values.block_3_v[1][0], 0);

    // 8
    values.block_4_v[0][0] = generate_number_not(values.block_2_v[0][0], values.block_2_v[0][1], values.block_3_v[0][0], values.block_3_v[1][0]);

    // 9
    values.block_4_v[1][0] = generate_number_not(values.block_3_v[0][0], values.block_3_v[1][0], values.block_4_v[0][0], 0);

    // 10
    values.block_4_v[0][1] = generate_number_not(values.block_2_v[0][0], values.block_2_v[0][1], values.block_4_v[0][0], 0);

    // 11
    values.block_4_v[1][1] = generate_number_not(values.block_4_v[0][0], values.block_4_v[1][0], values.block_4_v[0][1], 0);

    cout << "ALL 11 STEPS COMPLETED" << endl;

    // Make grid values 0
    int x, y;
    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
            grid.block_1[x][y] = 0;
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
            grid.block_2[x][y] = 0;
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
            grid.block_3[x][y] = 0;
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
            grid.block_4[x][y] = 0;
    }

    // Add some values into the grid
    grid.block_1[0][0] = values.block_1_v[0][0];
    grid.block_2[1][1] = values.block_2_v[1][1];
    grid.block_3[0][1] = values.block_3_v[0][1];
    grid.block_3[1][1] = values.block_3_v[1][1];
    grid.block_4[0][1] = values.block_4_v[0][1];
    grid.block_4[0][0] = values.block_4_v[0][0];
    grid.block_3[1][0] = values.block_3_v[1][0];
    grid.block_2[0][0] = values.block_2_v[0][0];

    cout << "Sudoku VALUES COMPUTED" << endl;
}

int Sudoku::random()
{
    auto now = chrono::high_resolution_clock::now();
    auto microseconds = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch()).count();

    srand(static_cast<unsigned>(microseconds));

    int num = (rand() % 4) + 1;
    return num;
}

int Sudoku::generate_number_not(int not_num_1, int not_num_2, int not_num_3, int not_num_4)
{
    cout << "GENERATE A RANDOM NUMBER" << endl;
    int num = 0;
    while(true)
    {
        num = random();
        cout << num << endl;

        if(num != not_num_1 && num != not_num_2 && num != not_num_3 && num != not_num_4)
            break;
    }

    cout << "RANDOM NUMBER GENERATED" << endl;
    return num;
}

void Sudoku::display()
{
    system("cls");

    cout << "SUDOKU" << endl;
    cout << "Grid values: " << endl;

    cout << "0 0  1 0   2 0  3 0" << endl;
    cout << "0 1  1 1   2 1  3 1" << endl;
    cout << endl;
    cout << "0 2  1 2   2 2  3 2" << endl;
    cout << "0 3  1 3   2 3  3 3" << endl;

    cout << endl;

    cout << "\t\t" << grid.block_1[0][0] << " " << grid.block_1[1][0] << "   " << grid.block_2[0][0] << " " << grid.block_2[1][0] << endl;
    cout << "\t\t" << grid.block_1[0][1] << " " << grid.block_1[1][1] << "   " << grid.block_2[0][1] << " " << grid.block_2[1][1] << endl;
    cout << endl;
    cout << "\t\t" << grid.block_3[0][0] << " " << grid.block_3[1][0] << "   " << grid.block_4[0][0] << " " << grid.block_4[1][0] << endl;
    cout << "\t\t" << grid.block_3[0][1] << " " << grid.block_3[1][1] << "   " << grid.block_4[0][1] << " " << grid.block_4[1][1] << endl;
}

void Sudoku::input(int x, int y, int value)
{   
    // Find block
    int block;
    if(x == 0 || x == 1)
    {
        block = 1;
        if(y == 2 || y == 3)
            block = 3;
    }

    else if(x == 2 || x == 3)
    {
        block = 2;
        if(y == 2 || y == 3)
            block = 4;
    }

    // Add the value to the grid
    if(block == 1)
    {
        if(grid.block_1[x][y] == 0)
            grid.block_1[x][y] = value;
    }

    if(block == 2)
    {
        int new_x;
        if(x == 2)
            new_x = 0;
        else if(x == 3)
            new_x = 1;
        
        if(grid.block_2[new_x][y] == 0)
            grid.block_2[new_x][y] = value;
    }

    if(block == 3)
    {
        int new_y;
        if(y == 2)
            new_y = 0;
        else if(y == 3)
            new_y = 1;

        if(grid.block_3[x][new_y] == 0)
            grid.block_3[x][new_y] = value;
    }

    if(block == 4)
    {
        int new_x, new_y;
        if(x == 2)
            new_x = 0;
        else if(x == 3)
            new_x = 1;

        if(y == 2)
            new_y = 0;
        else if(y == 3)
            new_y = 1;

        if(grid.block_4[new_x][new_y] == 0)
            grid.block_4[new_x][new_y] = value;
    }
}

bool Sudoku::check()
{   
    int x, y, wrongs = 0;
    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
        {
            if(grid.block_1[x][y] != values.block_1_v[x][y])
                wrongs++;
        }
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
        {
            if(grid.block_2[x][y] != values.block_2_v[x][y])
                wrongs++;
        }
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
        {
            if(grid.block_3[x][y] != values.block_3_v[x][y])
                wrongs++;
        }
    }

    for(y = 0; y < 2; y++)
    {
        for(x = 0; x < 2; x++)
        {
            if(grid.block_4[x][y] != values.block_4_v[x][y])
                wrongs++;
        }
    }

    if(wrongs == 0)
        return 0;
    else
        return 1;
}

int main()
{
    Sudoku sudoku;
    int x = 0, y = 0, value = 0;

    while(true)
    {
        sudoku.display();

        cout << "Enter co-ordinate and value (Enter 4 for submit): ";
        cin >> x >> y >> value;

        if(x == 4)
        {
            if(sudoku.check() == 0)
            {
                cout << "WELL DONE!, you have completed the game successfully" << endl;
                break;
            }
            else
            {
                cout << "TRY AGAIN" << endl;
                break;
            }
        }

        sudoku.input(x, y, value);
    }

    return 0;
}