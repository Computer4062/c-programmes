#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string>

// 25 x 23
std::string map =
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00010000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000"
"00000000000000000000000000000000000000000000000000";

int birdPosY = 11;
int score = 0;
bool easy = false;

// Game object
const char bird = '>';
const char airspace = ' ';
const char pillar = '|';

void Draw()
{
	system("cls");

	for (int y = 0; y < 23; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			if (map[50*y + x] == '1')
			{
				std::cout << bird;
			}
			else if (map[50*y + x] == '0')
			{
				std::cout << airspace;
			}
			else if (map[50 * y + x] == '2')
			{
				std::cout << pillar;

				// Move pillar 1 step to the left
				if (x != 1)
				{
					// Remove pillar from current location
					map[50*y + x] = '0';

					// Write pillar into new location
					map[50*y + (x - 1)] = '2';
				}
				else
				{
					map[50*y + x] = '0';
				}
			}
		}

		std::cout << std::endl;
	}

	std::cout << "Score: " << score;
}

void CreatePillars()
{
	// Generate the height of the pillar
	// 1. If difficulty is set to hard
	int p;
	if (!easy)
	{
		p = rand() % 5 + 5;
	}
	// 2. If difficulty is set to easy
	else
	{
		p = rand() % 10;
	}

	// Write the pillars into the map
	// 1. Draw top pillars
	for (int i = 0; i < p; i++)
	{
		map[50 * i + 49] = '2';
		map[50 * i + 48] = '2';
	}

	// 2. Draw bottom pillars
	for (int i = 0; i < p; i++)
	{
		map[50*(22 - i) + 49] = '2';
		map[50*(22 - i) + 48] = '2';
	}
}

bool Logic()
{
	// Check for collisions
	// 1. Downward collisions
	if (birdPosY != 22)
	{
		if (map[50 * (birdPosY + 1) + 3] == '2')
		{
			return false;
		}
	}

	// 2. Upward collisions
	if (birdPosY != 0)
	{
		if (map[50 * (birdPosY - 1) + 3] == '2')
		{
			return false;
		}
	}

	// 3. Sideward collisions
	if (map[50 * birdPosY + 4] == '2')
		return false;


	// Move bird 1 step down
	// 1. Remove bird
	map[50*birdPosY + 3] = '0';

	// 2. Write bird in new space
	if (birdPosY != 22)
	{
		birdPosY++;
		map[50 * birdPosY + 3] = '1';
	}
	else
		return false; // If bird fell down

	// Key presses
	if (_kbhit())
	{
		switch (_getch())
		{
			// If space bar pressed move bird up
			case ' ':
				if (birdPosY > 1)
				{
					// Remove from original location
					map[50*birdPosY + 3] = '0';
					
					// Update to new position
					birdPosY -= 2;
					map[50*birdPosY + 3] = '1';
				}

				break;

			// If (q) pressed exit
			case 'q':
				return false;
		}
	}

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

	CoutCentered("FLAPPY BIRD - TERMINAL EDITION");
	CoutCentered("Your bird is flying through a pathernon in the heavens, make sure it doesn't hit the columns and fall back to earth");
	CoutCentered("Hit (e) for easy");
	CoutCentered("Hit (h) for hard");

	std::cout << "Hit (q) for quit and (SPACE) to flap the bird" << std::endl;

	bool quit = false;
	while (!quit)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
				case 'e':
					easy = true;
					quit = true;
					break;

				case 'h':
					easy = false;
					quit = true;
					break;
			}
		}
	}
}

bool EndScreen()
{
	system("cls");

	CoutCentered("GAME OVER!");
	CoutCentered("Score = " + std::to_string(score));
	CoutCentered("Hit (p) to play again and (q) to exit");

	bool quit = false;
	while (!quit)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'p':
				{
					map =
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00010000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000"
						"00000000000000000000000000000000000000000000000000";

					birdPosY = 11;
					score = 0;
					easy = false;

					return true;
				}

				break;

			case 'q':
				return false;
				break;
			}
		}
	}
}

int main()
{
	int count = 10;
	int scorefactor = 0;
	bool startScoreCount = false;

	IntroScreen();

	while (true)
	{
		Draw();
		if (!Logic())
		{
			// If play again selected
			if (EndScreen() == true)
				IntroScreen();

			// If quit selected
			else
				break;
		}

		if (count == 10)
		{
			CreatePillars();
			count = 0;
		}
		else
		{
			count++;
		}

		if (startScoreCount == false)
		{
			scorefactor++;

			if (scorefactor == 48)
			{
				startScoreCount = true;
				scorefactor = 10;
			}
		}
		else
		{
			if (scorefactor == 10)
			{
				score++;
				scorefactor = 0;
			}
			else
			{
				scorefactor++;
			}
		}

		Sleep(80);
	}

	return 0;
}