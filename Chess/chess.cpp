#include <SDL.h>
#include <iostream>
#include <string>
#include "chess.h"

using namespace std;

void Chess::FindSquareXY(int &x, int &y, int square_x, int square_y)
{
    int i = 12; // initial value

    x = i + (square_x * 52);
    y = i + (square_y * 52);
    
}

void Chess::RenderPieces(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);

    // Load the chess board image
    SDL_Surface* board_image = NULL;

    board_image = SDL_LoadBMP("src/img/board.bmp");

    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, board_image);

    SDL_RenderCopy(renderer, image_texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(board_image);
    SDL_DestroyTexture(image_texture);

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            string piece_name = " ";
            string pos = positions[y][x];

            if(pos == "N") piece_name = "none"; // empty square
            else if(pos == "wR") piece_name = "w_rook";
            else if(pos == "wKn") piece_name = "w_knite";
            else if(pos == "wB") piece_name = "w_bishop";
            else if(pos == "wQ") piece_name = "w_queen";
            else if(pos == "wK") piece_name = "w_king";
            else if(pos == "wP") piece_name = "w_pawn";

            else if(pos == "bR") piece_name = "b_rook";
            else if(pos == "bKn") piece_name = "b_knite";
            else if(pos == "bB") piece_name = "b_bishop";
            else if(pos == "bQ") piece_name = "b_queen";
            else if(pos == "bK") piece_name = "b_king";
            else if(pos == "bP") piece_name = "b_pawn";

            if(piece_name != "none" && piece_name != " "){
                //cout << "positions[" << x << "] [" << y << "] | piece name: " << piece_name;

                int X, Y;
                FindSquareXY(X, Y, x, y);

                //cout << "| (" << X << "," << Y << ")" << endl;

                // image surface -> texture -> rectangle -> copy to a rectangle -> render rectangle
                string image_name = "src/img/" + piece_name + ".bmp";
                SDL_Surface* image = SDL_LoadBMP(image_name.c_str());

                if(!image)
                {
                    cout << "image: " << SDL_GetError() << endl;
                }

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
                if(!texture)
                {
                    cout << "Texture: " << SDL_GetError() << endl;
                }

                SDL_Rect rect = {X, Y, image->w, image->h};

                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
            }
        }
    }
}

bool Chess::Move(int x, int y)
{   
    if(x == picked_piece_x && y == picked_piece_y)
        return 1;

    if((y == 0 || y == 7) && (picked_piece == "wP" || picked_piece == "bP"))
    {
        string piece, name;
        cout << "\nWhat piece do you want: ";
        cin >> piece;

        cout << piece << "\t" << player << endl;

        if(piece[1] == 'n' || piece[1] == 'N')
        {
            name = player + "Kn";
        }
        else 
        {
            piece[0] = toupper(piece[0]);
            name = player + piece[0];
        }

        if(player == 'w') 
        {
            player = 'b';
            round_complete = false;
        }
        else if(player == 'b')
        {
            player = 'w';
            round_complete = true;
        }
        
        cout << name << endl;
        DisplayMove(x, y);
        positions[y][x] = name;
    }
    else if(positions[y][x][0] != player)
    {
        if(player == 'w') 
        {
            player = 'b';
            round_complete = false;
        }
        else if(player == 'b')
        {
            player = 'w';
            round_complete = true;
        }
        DisplayMove(x, y);
        positions[y][x] = picked_piece;
    }
    else
    {
        return 1;
    }

    return 0;
}

void Chess::DisplayMove(int x, int y)
{
    char letters[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    char letter = letters[x];

    int number_map[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    int num = number_map[y];

    bool captured = false;
    if(positions[y][x] != "N") 
        captured = true;

    if(picked_piece == "wKn" || picked_piece == "bKn")
    {
        if(captured)
            cout << "Nx" << letter << num << "\t";
        else
            cout << "N" << letter << num << "\t";
    }
    else if(picked_piece == "wP" || picked_piece == "bP")
    {
        if(captured)
            cout << letters[picked_piece_x] << "x" << letter << num << "\t";
        else
            cout << letter << num << "\t";
    }
    else
    {
        if(captured)
            cout << picked_piece[1] << "x" << letter << num << "\t";
        else
            cout << picked_piece[1] << letter << num << "\t";
    }

    if(round_complete)
    {
        cout << endl;
    }
} 

void Chess::Run()
{

    SDL_Init(SDL_INIT_VIDEO);

    // Create the window
    SDL_Window* window = NULL;

    window = SDL_CreateWindow(
        "Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        427, 427,
        0
    );

    if(NULL == window)
    {
        SDL_Quit();
        cout << SDL_GetError() << endl;
        exit(1);
    }

    // Create the renderer
    SDL_Renderer* renderer = NULL;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(NULL == renderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        cout << "Window" << SDL_GetError() << endl;
        exit(1);
    }

    // Display all the pieces
    RenderPieces(renderer);

    // Start the game
    SDL_Event event;
    bool quit = false;

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(SDL_QUIT == event.type)
                quit = true;

            if(SDL_MOUSEBUTTONDOWN == event.type)
            {
                int x = event.button.x;
                int y = event.button.y;

                int sx = (int) ((x - 26) / 52);
                int sy = (int)((y - 26) / 52);

                string piece = positions[sy][sx];

                if(piece[0] == player && piece != "\n" && piece_picked == false)
                {
                    positions[sy][sx] = "N";
                    piece_picked = true;
                    picked_piece = piece;
                    picked_piece_x = sx;
                    picked_piece_y = sy;
                    RenderPieces(renderer);
                }

                else if(piece_picked)
                {
                    placed_piece = positions[sy][sx];
                    int state = Move(sx, sy);
                    if(state == 0)
                    {
                        placed_x = sx;
                        placed_y = sy;
                        piece_picked = false;
                    }

                    RenderPieces(renderer);
                }
            }

            if(SDL_KEYDOWN == event.type)
            {
                if(event.key.keysym.sym == SDLK_z)
                {
                    positions[placed_y][placed_x] = placed_piece;
                    positions[picked_piece_y][picked_piece_x] = picked_piece;

                    if(player == 'w') player = 'b';
                    else if(player == 'b') player = 'w';

                    RenderPieces(renderer);
                }
            }
        }
        
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
}

int main(int argc, char* argv[])
{
    Chess chess;
    chess.Run();
}