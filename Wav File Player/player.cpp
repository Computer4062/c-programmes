#include <iostream>
#include <windows.h>
#include <fstream>

int main()
{
  std::string filename;

  std::cout << "Audio file: ";
  std::cin >> filename;

  std::ifstream file(filename); 

  if(!file.is_open()){
    std::cout << "Could not find file: " << filename << std::endl;
    return 1;
  }
  
  /*
   * Play Sound Paramters:
   * filename, handle modules, flags
   * SND_SYNC plays the sound file syncronus which means it waits for the sound file
   * to finish playing before returning to the calling code
  */ 

  if(!PlaySound(filename.c_str(), NULL, SND_SYNC)){
	std::cout << "Some thing went wrong" << std::endl;
  }

  file.close();
  return 0;
}

// Command to compile with g++: g++ player.cpp -lwinmm
