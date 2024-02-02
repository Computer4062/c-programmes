#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

// 30 x 30
string map = 
"##############################"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000000000000000000#"
"#0000000000000100000000000000#"
"##############################";

char spaceshipChar = 'O';
char laserbeamsChar = '|';
char enemyMissilesChar = 'Q';
char enemyshipChar = 'X';

// x co-ordinate
int spaceshipPos = 14;

// List of enemyships
vector<int> enemyShips;

// List of enemy missiles
vector<vector<int>> enemyMissiles;

// List of missile count down
vector<int> missileCountDown;

// Footnotes
string footnotes = " ";
int score = 0;

// Laser related specs
int lasers = 5; // Laser power currently ready
int laserCountDown = 50; // Amount of time it takes for the laser to get ready

bool gameover = false;

void Display()
{
	system("cls");

	// Update missile positions
	if(enemyMissiles.size() > 0)
	{
		for(int i = 0; i < enemyMissiles.size(); i++)
		{
			int x = enemyMissiles[i][0];
			int y = enemyMissiles[i][1];

			// Erase enemy missile if it already passed by
			if(y > 27)
			{
				enemyMissiles.erase(enemyMissiles.begin() + i);
				
				// Erase missile off display
				map[30*y + x] = '0';
			}
			else
			{
				// Erase enemy missiles current position
				map[30*y + x] = '0';
		
				// Write enemy missile in new position
				map[30*(y + 1) + x] = enemyMissilesChar;
		
				// Update enemy missiles next location
				enemyMissiles[i] = {x, y + 1};		
			}
		}
	}

	for(int y = 0; y < 30; y++)
	{
		for(int x = 0; x < 30; x++)
		{
			// If it is part of the border
			if(map[30*y + x] == '#')
				cout << '#';
			
			// If it is a space
			else if(map[30*y + x] == '0')
				cout << ' ';
			
			// Spaceship
			else if(map[30*y + x] == spaceshipChar)
			{
				cout << spaceshipChar;

				// Detect collisions
				if(map[30*(y - 1) + x] == enemyMissilesChar || map[30*(y - 1) + x] == enemyshipChar)
				{	
					gameover = true;
					break;
				}
			}
			
			// Laser beams
			else if(map[30*y + x] == laserbeamsChar)
			{
				// If missile has missed
				if(y == 1)
				{
					// Stop firing the laser beam
					for(int yCord = 0; yCord < 30; yCord++)
					{
						if(map[30*yCord + x] == laserbeamsChar)
							map[30*yCord + x] = '0';
					}
				}
				else
				{
					// If laser beam has hit an object
					if(map[30*(y - 1) + x] == enemyMissilesChar || map[30*(y - 1) + x] == enemyshipChar)
					{
						// If enemy missile has been hit
						if(map[30*(y - 1) + x] == enemyMissilesChar)
						{
							// Find the missile that has been hit
							int i = 0;
							vector<int> cords;
							for(i; i < enemyMissiles.size(); i++)
							{
								cords = {x, y - 1};
								if(enemyMissiles[i] == cords)
									break;
							}

							// Destroy the missile
							enemyMissiles.erase(enemyMissiles.begin() + i);
							map[30*(y - 1) + x] = '0';

							// Increment score by 1
							score++;
						}

						// If enemy ship has been hit
						else if(map[30*(y - 1) + x] == enemyshipChar)
						{
							// Find the ship that has been hit
							int i = 0;
							for(i; i < enemyShips.size(); i++)
							{
								if(enemyShips[i] == x)
									break;
							}

							// Destroy the ship
							enemyShips.erase(enemyShips.begin() + i);
							missileCountDown.erase(missileCountDown.begin() + i);
							map[30*(y - 1) + x] = '0';

							// Increment score by 2
							score += 2;
						}


						// Stop firing the laser beam
						for(int y = 0; y < 30; y++)
						{
							for(int x = 0; x < 30; x++)
							{
								if(map[30*y + x] == laserbeamsChar)
									map[30*y + x] = '0';
							}
						}
					}
					else
					{
						map[30*(y - 1) + x] = laserbeamsChar;
					}

					cout << laserbeamsChar;
				}
			}
			
			// Enemy missile
			else if(map[30*y + x] == enemyMissilesChar)
				cout << enemyMissilesChar;
			
			// Enemy ship
			else if(map[30*y + x] == enemyshipChar)
				cout << enemyshipChar;
		}
		
		cout << endl;
	}

	cout << "Score: " << score << endl;
	cout << footnotes;
}

void LaunchEnemyShip()
{
	// Select position
	while(true)
	{
		int pos = rand() % 28;
		if(pos == 0) pos = 1;
		
		if(enemyShips.size() > 0)
		{
			bool used = false;
			for(int i = 0; i < enemyShips.size(); i++)
			{
				if(enemyShips[i] == pos)
				{
					used = true;
				}
			}

			if(!used)
			{
				enemyShips.push_back(pos);
				map[30*1 + pos] = enemyshipChar;

				break;
			}
		}
		else
		{
			enemyShips.push_back(pos);
			map[30*1 + pos] = enemyshipChar;

			break;
		}
	}

	missileCountDown.push_back(0);
}

void LaunchEnemyMissile(int i)
{	
		// Add the missile to the list
	vector<int> pos = {enemyShips[i], 2};
	enemyMissiles.push_back(pos);

	map[30*2 + enemyShips[i]] = enemyMissilesChar;
}

// Returns false if exit
bool Logic()
{
	// If laser power runs out
	if(lasers == 0)
	{
		// Get laser power ready
		if(laserCountDown == 0)
		{
			lasers = 5;
			laserCountDown = 50;

			footnotes = "GETTING RAY GUN READY, HOLD ON! " + to_string(laserCountDown);
		}
		else
		{
			laserCountDown--;
			footnotes = "GETTING RAY GUN READY, HOLD ON! " + to_string(laserCountDown);
		}

	}
	else
	{
		footnotes = "rays = " + to_string(lasers);
	}

	if(_kbhit())
	{
		switch(_getch())
		{
			// Left
			case 'a':
				map[30*28 + spaceshipPos] = '0';
				if(spaceshipPos != 1)
					spaceshipPos--;

				map[30*28 + spaceshipPos] = spaceshipChar;
	
				break;
			
			// Right
			case 'd':
				map[30*28 + spaceshipPos] = '0';
				if(spaceshipPos != 28)
					spaceshipPos++;

				map[30*28 + spaceshipPos] = spaceshipChar;
				
				break;
			
			// Fire laser beam
			case ' ':
				// If there is enough laser power
				if(lasers != 0)
				{
					map[30*(28 - 1) + spaceshipPos] = laserbeamsChar;

					footnotes = "rays = " + to_string(lasers);
					lasers--;
				}

				break;
			
			// Exit
			case 'e':
				return false;
				break;
		}
	}

	// Check for collisions
	if(gameover)
		return false;
	
	return true;
}

void CoutCentered(std::string text) {
	// This function will only center the text if it is less than the length of the console!
	// Otherwise it will just display it on the console without centering it.

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get the console handle.
	PCONSOLE_SCREEN_BUFFER_INFO lpScreenInfo = new CONSOLE_SCREEN_BUFFER_INFO(); // Create a pointer to the Screen Info pointing to a temporal screen info.
	GetConsoleScreenBufferInfo(hConsole, lpScreenInfo); // Saves the console screen info into the lpScreenInfo pointer.
	COORD NewSBSize = lpScreenInfo->dwSize; // Gets the size of the screen
	if (NewSBSize.X > text.size()) {
		int newpos = ((NewSBSize.X - text.size()) / 2); // Calculate the number of spaces to center the specific text.
		for (int i = 0; i < newpos; i++) std::cout << " "; // Prints the spaces
	}
	std::cout << text << std::endl; // Prints the text centered :]
}

void IntroScreen()
{
	system("cls");

	CoutCentered("STELLAR COMBAT");
	CoutCentered("Alien spaceships are attacking the human civilization");
	CoutCentered("Use your spaceships death rays to destroy the alien ships and protect humanity!");

	cout << "\nPress: (a) for left movement | (d) for right movement | (e) for exit | (SPACE) to fire death ray" << endl;
	system("pause");
}

int main()
{	
	// Start Intro screen
	IntroScreen();

	footnotes = "rays = " + to_string(lasers);
	int enemyShipCountDown = 0;

	bool exit = false;

	while(!exit)
	{
		Display();
		if(!Logic())
		{
			// End screen
			system("cls");
			CoutCentered("GAME OVER!");
			CoutCentered("SCORE = " + to_string(score));
			CoutCentered("Press (e) to exit | (SPACE) to play again...");

			bool exitloop = false;
			while(!exitloop)
			{
				if(_kbhit())
				{
					switch(_getch())
					{
						// Exit
						case 'e':
							exitloop = true;
							exit = true;
							break;

						// Replay
						case ' ':
							{
								map = 
								"##############################"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000000000000000000#"
								"#0000000000000100000000000000#"
								"##############################";

								spaceshipPos = 14;
								enemyShips = {};
								enemyMissiles = {};
								missileCountDown = {};
								footnotes = " ";
								score = 0;
								lasers = 5;
								laserCountDown = 20;
								gameover = false;

								// Exit the loop
								exitloop = true;
							}

							break;
					}
				}
			}
		}
	
		// Enemy ships are launching there missiles
		if(enemyShips.size() > 0)
		{
			for(int i = 0; i < missileCountDown.size(); i++)
			{
				// If next missile ready, FIRE
				if(missileCountDown[i] == 0)
				{
					LaunchEnemyMissile(i);
					missileCountDown[i] = 40;
				}
				else
				{
					int count = missileCountDown[i];
					missileCountDown[i] = count - 1; // Get the next missile ready
				}
			}
		}

		// Spawn an enemyship
		if(enemyShipCountDown == 0)
		{
			LaunchEnemyShip();
			enemyShipCountDown = 50;
		}
		else
		{
			enemyShipCountDown--;
		}

		Sleep(40);
	}

	return 0;
}
