#include <fstream>

#include "VM.h"

bool checkFileType(char* data);

int main () {
  std::streampos size;
  std::ifstream file ("example.dat", std::ios::in|std::ios::binary|std::ios::ate);
  char* filedata;
  if (file.is_open()){
    size = file.tellg();
    filedata = new char [size];
    file.seekg (0, std::ios::beg);
    file.read (filedata, size);
    file.close();
    if(checkFileType(filedata)){
      std::cout << "FILE VERIFIED\n";
      std::cout << filedata << "\n";
      filedata = filedata+(16 * sizeof(char));
      std::cout << filedata <<  " " << (uint)size-16 << "\n";
      VM vm(filedata, (uint)size-16);
      vm.run();
    }
  }else{
    std::cout << "Unable to open file";
    return 0;
  }
  std::cin.get();
  return 0;
}

bool checkFileType(char* data){
  const char* sig = "BrandonKirincich";
  return memcmp(sig, data, 16) == 0;
}