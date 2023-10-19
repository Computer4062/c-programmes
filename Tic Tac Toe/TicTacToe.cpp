#include <iostream>
#include <stdlib.h>

using namespace std;

char mat[3][3], p1, p2;

int  init();                // Initialize mat
void display();             // Display current game
bool input(char, int, int); // Player input
char check();               // Check if move is correct
bool draw();                // Check if game is a draw

int init()
{
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			mat[i][j] = '-';
}

void display()
{
	int i, j;
	system("cls");
	cout << "TIC TAC TOE \n\n Grid co-ordinates" << endl;

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
			cout << i << " " << j << "   ";

		cout << endl;
	}

	cout << "\n TIC TAC TOE board: " << endl << endl;

	for(i = 0; i < 3; i++)
	{
		cout << "\t";
		for(j = 0; j < 3; j++)
			cout << " " << mat[i][j] << "   ";

		cout << endl;
	}
}

bool input(char choice, int x, int y)
{
	if(x >= 0 && x <= 2 && y >= 0 && y <= 2)
	{
		if(mat[x][y] == '-')
		{
			mat[x][y] = choice;
			display();
			return 0;
		}
		else
			cout << "Error 1" << endl;
			return 1;
	}

	else
		cout << "Error 2" << endl;
		return 1;
}

char check()
{
	int  i, j;
	char temp;

	for(j = 0; j < 3; j++)
	{
		if(mat[0][j] != '-')
		{
			temp = mat[0][j];
			if(j == 0)
			{
				if(mat[0][1] == temp)
					if(mat[0][2] == temp)
						return temp;
				
				if(mat[1][0] == temp)
					if(mat[2][0] == temp)
						return temp;

				if(mat[1][1] == temp)
					if(mat[2][2] == temp)
						return temp;
			}
			else if(j == 1)
			{
				if(mat[1][1] == temp)
					if(mat[2][1] == temp)
						return temp;
			}
			else
			{
				if(mat[1][2] == temp)
					if(mat[2][2] == temp)
						return temp;
			}
		}
	}

	for(i = 1; i < 3; i++)
	{
		if(mat[i][0] != '-')
		{
			temp = mat[i][0];
			if(i == 1)
			{
				if (mat[1][1] == temp)
					if(mat[1][2] == temp)
						return temp;
			}
			else
			{
				if(mat[2][1] == temp)
					if(mat[2][2] == temp)
						return temp;

				if(mat[1][1] == temp)
					if(mat[0][2] == temp)
						return temp;
			}
		}
	}

	return 'f';
}

bool draw()
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(mat[j][i] == '-')
				return 0;
		}
	}

	return 1;
}

int main()
{
	char player = 'X';
	int x, y;
	init();
	display();

	do
	{
		// Get player input
		bool result;
		while(true)
		{
		
			cout << "It is " << player << "'s turn: ";
			cin >> x >> y; 

			result = input(player, x, y);

			if(result == 1)
			{
				cout << "Invalid position" << endl;
			}
			else
				break;
		}

		// Switch players
		if('X' == player)
		{
			player = 'O';
		}
		else 
		{	
			player = 'X';
		}

		// Check if game has ended
		if(draw())
		{
			cout << "DRAW" << endl;
			break;
		}

		char winner = check();
		if('f' != winner)
		{
			cout << winner << " wins the game!";
			break;
		}

	} while(true);

	return 0;
}