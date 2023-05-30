import os

os.system('g++ -Isrc/include -Lsrc/lib -o main CHIP8.cpp emulator.cpp interface.cpp -lmingw32 -lSDL2main -lSDL2')