#include <iostream>
#include <thread>
#include <string>
#include <ctime>
#include <fstream>
#include <chrono>

#include "chip8.h"

using namespace std;

unsigned char font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void CHIP8::initialize(){
    opcode = 0;
    I = 0;
    PC = 0x200;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;

    for(int i = 0; i < 16; i++){
        R[i] = 0;
        key[i] = 0;
        stack[i] = 0;
    }

    for(int i = 0; i < (32 * 64); i++)
        graphics[i] = 0;

    for(int i = 0; i < 4096; i++)
        memory[i] = 0;

    for(int i = 0; i < 80; i++)
        memory[i] = font[i];

    srand(time(0));
}

void CHIP8::LoadROM(const string &filename){
    ifstream file;
    file.open(filename, ios::in | ios::binary);

    if(!file.is_open()){
        cerr << "file could not be opened" << endl;
        exit(1);
    }

    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    char *buffer = new char[length];
    file.read(buffer, length);

    if(length < 4096 - 512){
        for(int i = 0; i < length; ++i)
            memory[512 + i] = buffer[i];
    }else{
        cerr << "File is too big" << endl;
        exit(1);
    }

    delete[] buffer;
    file.close();
}

void CHIP8::emulateCycle(){
    opcode  = memory[PC] << 8 | memory[PC + 1];

    switch (opcode & 0xF000){

        case 0x0000: // 0XYZ
            switch (opcode & 0x00FF) {
                case 0x00E0: // 00E0
                    for (int i = 0; i < 64 * 32; ++i){
                        graphics[i] = 0;
                    }
                    drawFlag = true;
                    PC += 2;
                    break;

                case 0x00EE: //00EE
                    sp -= 1;
                    PC = stack[sp];
                    PC += 2;
                    break;

                default:
                    std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                    exit(1);
            }
            break;

        case 0x1000:
            PC = (opcode & 0x0FFF);
            break;

        case 0x2000:
            stack[sp] = PC;
            PC = (opcode & 0x0FFF);
            sp += 1;
            break;

        case 0x3000:
            if (R[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                PC += 4;
            }
            else {
                PC += 2;
            }
            break;

        case 0x4000:
            if (R[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                PC += 4;
            }
            else {
                PC += 2;
            }
            break;

        case 0x5000:
            if ((opcode & 0x000F) == 0x0000){
                if (R[(opcode & 0x0F00) >> 8] == R[(opcode & 0x00F0) >> 4]){
                    PC += 4;
                }

                else {
                    PC += 2;
                }
                break;
            }
            else {
                std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                exit(1);
            }

            break;
        case 0x6000:
            R[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            PC += 2;
            break;

        case 0x7000:
            R[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            PC += 2;
            break;

        case 0x8000:
            switch (opcode & 0x000F){
                case 0x0000:
                    R[(opcode & 0x0F00) >> 8] = R[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0001:
                    R[(opcode & 0x0F00) >> 8] |= R[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0002:
                    R[(opcode & 0x0F00) >> 8] &= R[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0003:
                    R[(opcode & 0x0F00) >> 8] ^= R[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0004:
                    R[(opcode & 0x0F00) >> 8] += R[(opcode & 0x00F0) >> 4];
                    if (R[(opcode & 0x0F00) >> 8] > (0xFF - R[(opcode & 0x00F0) >> 4]))
                        R[0xF] = 1;
                    else
                        R[0xF] = 0;

                    PC += 2;
                    break;

                case 0x0005:
                    if (R[(opcode & 0x0F00) >> 8] > (R[(opcode & 0x00F0) >> 4]))
                        R[0xF] = 1;
                    else
                        R[0xF] = 0;

                    R[(opcode & 0x0F00) >> 8] -= R[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                case 0x0006:
                    R[0xF] = R[(opcode & 0x0F00) >> 8] & 0x0001;
                    R[(opcode & 0x0F00) >> 8] = R[(opcode & 0x0F00) >> 8] >> 1;
                    PC += 2;
                    break;

                case 0x0007:
                    if (R[(opcode & 0x0F00) >> 8] > (R[(opcode & 0x00F0) >> 4]))
                        R[0xF] = 1;
                    else
                        R[0xF] = 0;

                    R[(opcode & 0x0F00) >> 8] = R[(opcode & 0x00F0) >> 4] - R[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x000E:
                    R[0xF] = R[(opcode & 0x0F00) >> 8] >> 7;
                    R[(opcode & 0x0F00) >> 8] = R[(opcode & 0x0F00) >> 8] << 1;
                    PC += 2;
                    break;

                default:
                    std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                    exit(1);
            }
            break;

        case 0x9000:
            if ((opcode & 0x000F)== 0x0){
                if (R[(opcode & 0x0F00) >> 8] != R[(opcode & 0x00F0) >> 4])
                    PC += 4;
                else
                    PC += 2;
            }
            else {
                std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                exit(1);
            }
            break;

        case 0xA000:
            I = (opcode & 0x0FFF);
            PC += 2;
            break;

        case 0xB000:
            PC = R[0x0] + (opcode & 0x0FFF);
            break;

        case 0xC000:
            R[(opcode & 0x0F00) >> 8] = (rand() % (0x00FF + 1)) & (opcode & 0x00FF);
            PC += 2;
            break;

        case 0xD000:
        {
            unsigned short x, y, pixel, height;

            x = R[(opcode & 0x0F00) >> 8];
            y = R[(opcode & 0x00F0) >> 4];
            height = opcode & 0x000F;

            R[0xF] = 0;
            for(unsigned short row = 0; row < height; ++row){
                pixel = memory[I + row];
                for(unsigned short column = 0; column < 8; ++column){
                    if ((pixel & (0x0080 >> column)) != 0){
                        unsigned short posPixel = ((x + column)%64) + (((y + row)%32) * 64);
                        if(graphics[posPixel] == 1){
                            R[0xF] = 1;
                        }


                        graphics[posPixel] ^= 1;
                    }
                }
            }

            drawFlag = true;
            PC += 2;
        }
            break;

        case 0xE000:
            if ((opcode & 0x00FF) == 0x009E){

                if (key[R[(opcode & 0x0F00) >> 8]] != 0)
                    PC += 4;
                else
                    PC += 2;
                break;
            }
            else if ((opcode & 0x00FF) == 0x00A1){

                if (key[R[(opcode & 0x0F00) >> 8]] == 0)
                    PC += 4;
                else
                    PC += 2;
                break;
            }
            else {
                 std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                 exit(1);
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007:
                    R[(opcode & 0x0F00) >> 8] = delayTimer;
                    PC += 2;
                    break;

                case 0x000A:
                {
                    bool keyPressed = false;
                    for(int i = 0; i < 16; ++i){
                        if (key[i] != 0){
                            keyPressed = true;
                            R[(opcode & 0x0F00) >> 8] = i;
                            break;
                        }
                    }
                    if(!keyPressed) return;
                    PC += 2;
                }
                    break;

                case 0x0015:
                    delayTimer = R[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x0018:
                    soundTimer = R[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x001E:
                    I += R[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                case 0x0029:
                    I = R[(opcode & 0x0F00) >> 8] * 5;
                    PC += 2;
                    break;

                case 0x0033:
                    memory[I]     = (R[(opcode & 0x0F00) >> 8] / 100);
                    memory[I + 1] = (R[(opcode & 0x0F00) >> 8] % 100) / 10;
                    memory[I + 2] = (R[(opcode & 0x0F00) >> 8] % 10);
                    PC += 2;
                    break;

                case 0x0055:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i){
                        memory[I + i] = R[i];
                    }
                    PC += 2;
                    break;

                case 0x0065:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i){
                        R[i] = memory[I + i];
                    }
                    PC += 2;
                    break;

                default:
                    std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
                    exit(1);
            }
            break;

        default:
            std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::dec << std::endl;
            exit(1);
    }

    if(delayTimer > 0)
        --delayTimer;

    if(soundTimer > 0){
        if(soundTimer == 1)
            beepFlag = 1;

        --soundTimer;
    }
}