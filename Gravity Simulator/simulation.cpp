#include "SDL.h"
#include <iostream>

int gravity = 0;

bool isMouseInRect(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

int FindGravity(SDL_Window* window, SDL_Renderer* renderer)
{
	// Create the buttons
    SDL_Rect down = { 100, 100, 100, 50 };
    SDL_Rect up   = { 400, 100, 100, 50 };

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &down);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &up);

	SDL_RenderPresent(renderer);

	// Start window
	SDL_Event event;
	bool quit = false;
	bool clicked = false;

	SDL_SetWindowTitle(window, std::to_string(gravity).c_str());

	while(!quit){
		while(SDL_PollEvent(&event)){
			if(SDL_QUIT == event.type)
				quit = true;

			if(SDL_MOUSEBUTTONDOWN == event.type){
				int x = event.button.x;
				int y = event.button.y;

				if(isMouseInRect(x, y, up)) {
					gravity++;
					SDL_SetWindowTitle(window, std::to_string(gravity).c_str());
				}
				
				if(isMouseInRect(x, y, down)){
					if(gravity != 0)
						gravity--;	

					SDL_SetWindowTitle(window, std::to_string(gravity).c_str());
				}	
			}
		}
	}

	return true;
}

int main(int argc, char* argv[])
{
	// Create a window
	SDL_Window* window = SDL_CreateWindow(
		"Simulator", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		800, 600, 
		0
	);

	// Create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Change background to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// Start window
	SDL_Event event;
	bool quit = false;
	bool clicked = false;

	int x = 0; 
	int y = 0;

	bool gravityWindowClosed = false;

	while(!quit){
		while(SDL_PollEvent(&event)){
			if(SDL_QUIT == event.type)
				quit = true;

			if(SDL_MOUSEBUTTONDOWN == event.type){
				x = event.button.x;
				y = event.button.y;

				clicked = true;
			}
		}

		if(clicked){
			if(!gravityWindowClosed)
				gravityWindowClosed = FindGravity(window, renderer);

			while (true){
				// Clear the screen
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);

				std::cout << "In progress: " << "( " << x << " , " << y << " )" << std::endl;

				// Create a rectangle
				y += gravity;

				if(y > 550)
					break;

				SDL_Rect rect;
				rect.x = x;
				rect.y = y;
				rect.w = 50;
				rect.h = 50;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				SDL_RenderFillRect(renderer, &rect);
				SDL_RenderPresent(renderer);

				SDL_Delay(10);
			}

			clicked = false;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}