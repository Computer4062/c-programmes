#include "SDL2/SDL.h"
#include <cmath> 
#include <iostream>

#define WIDTH       800   // Screen width
#define HEIGHT      600   // Screen height
#define NUM_PLANETS 3     // Including the sun
#define G           0.1   // Gravitational constant

struct CelestialBody {
	double x, y;     // Position
	double vx, vy;   // Velocity
	double mass;     // Mass
	double radius;   // Radius for drawing purposes
	SDL_Color color; // Color for drawing purposes
};

CelestialBody planets[NUM_PLANETS];

void InitializePlanets()
{
	  planets[0] = {WIDTH / 2.0, HEIGHT / 2.0, 0.0, 0.0, 10000.0, 30, {255, 255, 0, 255}};

    // Initialize other planets (positions, velocities, masses, and colors)
    // For simplicity, we'll use hypothetical values here.
    // The mass and speed of each planet will be scaled relative to the Sun's mass and speed.
    // For example, if Earth's mass is 5.972e24 kg, you can set it as 5.972e24/1000.0, etc.
    // The radius will be scaled to make the planets visible on the screen.

    // Planet 1 (Mercury)
    planets[1] = {WIDTH / 2.0 - 200.0, HEIGHT / 2.0, 0.0, -2.0, 0.1, 5, {192, 192, 192, 255}};

    // Planet 2 (Venus)
    planets[2] = {WIDTH / 2.0 + 300.0, HEIGHT / 2.0, 0.0, -1.5, 0.2, 10, {244, 164, 96, 255}};

    // Planet 3 (Earth)
    planets[3] = {WIDTH / 2.0 - 400.0, HEIGHT / 2.0, 0.0, 1.2, 0.3, 15, {0, 0, 255, 255}};
}

void UpdatePhysics()
{
	// Update the positions and velocities of planest based on gravity
	for(int i = 0; i < NUM_PLANETS; ++i){
		for(int j = 0; j < NUM_PLANETS; ++j){
			if(i == j) continue;

			// Calculate distance between each planet
			double dx = planets[j].x - planets[i].x;
			double dy = planets[j].y - planets[i].y;

			double distanceSquared = dx * dx + dy * dy;
			double distance = std::sqrt(distanceSquared);

			// Calculate force and acceleration
			double force = G * planets[i].mass * planets[j].mass / distanceSquared;
			double acceleration = force / planets[i].mass;

			// Calculate accerleration for each planet
			double ax = acceleration * dx / distance;
			double ay = acceleration * dy / distance;

			planets[i].vx += ax;
			planets[i].vy += ay;
		}
	}

	// Update the positions based on velocities
	for(int i = 0; i < NUM_PLANETS; ++i){
		planets[i].x += planets[i].vx;
		planets[i].y += planets[i].vy;
	}
}

// Use Bresenhams circle algorithm to draw the circles
void DrawCircle(SDL_Renderer* renderer, int cx, int cy, int radius, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;

	while (y >= x){
		// Draw the points in all octants
		SDL_RenderDrawPoint(renderer, cx + x, cy + y);
		SDL_RenderDrawPoint(renderer, cx + x, cy - y);
		SDL_RenderDrawPoint(renderer, cx - x, cy + y);
		SDL_RenderDrawPoint(renderer, cx - x, cy - y);
		SDL_RenderDrawPoint(renderer, cx + y, cy + x);
		SDL_RenderDrawPoint(renderer, cx + y, cy - x);
		SDL_RenderDrawPoint(renderer, cx - y, cy + x);
		SDL_RenderDrawPoint(renderer, cx - y, cy - x);

		if(d < 0){
			d += 4 * x + 6;
		} else {
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}

void Draw(SDL_Renderer* renderer)
{
	// Clear the screen 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // renderer, r, g, b, a
	SDL_RenderClear(renderer);

	// Draw the sun and the planets
	for(int i = 0; i < NUM_PLANETS; ++i){
		SDL_SetRenderDrawColor(renderer, planets[i].color.r, planets[i].color.b, planets[i].color.g, planets[i].color.a);

		int x = static_cast<int>(planets[i].x);
		int y = static_cast<int>(planets[i].y);
		int radius = static_cast<int>(planets[i].radius);

		DrawCircle(renderer, x, y, radius, planets[i].color);
	}

	// Show the renderered frame
	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	// Create the window
	SDL_Window* window = NULL;

	window = SDL_CreateWindow(
		"Solar system simulation", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		WIDTH, HEIGHT, 
		0
	);

	// Check if window was created properly
	if(NULL == window){
		std::cerr << "Error: Unable to create window: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create renderer
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Check if renderer was created properly
	if(NULL == renderer){
		std::cerr << "Error: Unable to create renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		return -1;
	}

	InitializePlanets();

	SDL_Event event;
	bool quit = false;

	while(!quit){
		while(SDL_PollEvent(&event)){
			if(SDL_QUIT == event.type)
				quit = true;

			if(SDL_KEYDOWN == event.type){
				if(SDLK_ESCAPE == event.key.keysym.sym)
					quit = true;
			}
		}

		UpdatePhysics();
		Draw(renderer);

		SDL_Delay(10); // Adjust to control the simulation
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}