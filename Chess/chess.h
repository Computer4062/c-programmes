#include <SDL.h>
#include <string>

class Chess
{
    public:
        Chess()
        {
        }

        ~Chess()
        {
        }

        void RenderPieces(SDL_Renderer* renderer);
        void FindSquareXY(int &x, int &y, int square_x, int square_y);
        bool Move(int x, int y);
        void Run();
        void DisplayMove(int x, int y);

    private:
        std::string positions[8][8] =
        {
            {"bR", "bKn", "bB", "bK", "bQ", "bB", "bKn", "bR"}, // black
            {"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"}, // black
            {"N", "N", "N", "N", "N", "N", "N", "N"},
            {"N", "N", "N", "N", "N", "N", "N", "N"},
            {"N", "N", "N", "N", "N", "N", "N", "N"},
            {"N", "N", "N", "N", "N", "N", "N", "N"},
            {"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"}, // white
            {"wR", "wKn", "wB", "wK", "wQ", "wB", "wKn", "wR"}, // white
        };

        const int x_factor = 2;
        const int y_factor = 2;
        
        bool piece_picked = false;
        std::string picked_piece;
        std::string placed_piece;
        int picked_piece_x;
        int picked_piece_y;
        int placed_x, placed_y;
        char player = 'w';
        bool round_complete = false;
};