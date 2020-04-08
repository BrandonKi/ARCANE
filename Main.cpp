#include <fstream>

#include "VM.h"

int main () {
  std::streampos size;
  std::ifstream file ("example.dat", std::ios::in|std::ios::binary|std::ios::ate);
  char* filedata;
  if (file.is_open())
  {
    size = file.tellg();
    filedata = new char [size];
    file.seekg (0, std::ios::beg);
    file.read (filedata, size);
    file.close();
  }else{
    std::cout << "Unable to open file";
    return 0;
  }
  // std::cout << filedata << " " << size << "\n";
  VM vm(filedata, size);
  vm.run();
  std::cin.get();
  return 0;
}