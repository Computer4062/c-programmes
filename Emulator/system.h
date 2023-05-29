#include <string>

using namespace std;

class CHIP8{
    public:
        //empty default constructor
        CHIP8();
        ~CHIP8();

        //clear the memory

        void intialise();

        //load the program into memore
        //return true of loaded 
        //parm file name path to ROM 

        bool loadROM(const string& filename);

        //emulate one cycle of the CPU
        void emulateCycle();

        //graphics screen 32 * 64 pixels
        unsigned char graphics[32 * 64];
        unsigned char key[16]; //registers

        bool drawFlag;
        bool beepFlag; //sound

    private:
        unsigned short opcode;
        unsigned short I;
        unsigned short PC;
        unsigned short stack[16];
        unsigned short sp;

        unsigned char R[16];
        unsigned char memory[4096];
        unsigned char delayTimer;
        unsigned char soundTimer;
};