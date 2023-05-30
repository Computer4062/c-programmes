#include <string>

class CHIP8{
    public:
        CHIP8(){

        }

        ~CHIP8(){

        }

        void initialize();
        void LoadROM(const std::string &filename);

        void emulateCycle();

        unsigned graphics[32 * 64];
        unsigned char key[16];

        bool drawFlag;
        bool beepFlag;

    private:
        unsigned short opcode;
        unsigned short I;
        unsigned short PC;
        unsigned short sp;
        unsigned short stack[16];

        unsigned char memory[4096];
        unsigned char R[16];
        unsigned char delayTimer;
        unsigned char soundTimer;
};