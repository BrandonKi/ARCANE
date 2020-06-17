#include <fstream>
#include <chrono> 
#include "VM.h"

clock_t start, end; 
#define TIMER_START start = clock()
#define TIMER_STOP std::cout << "Time taken by program is : " << ((double(clock() - start) / double(CLOCKS_PER_SEC)) * 1000) << " milliseconds " << std::endl; 

inline bool checkFileType(char* data);
// inline char* appendExtension(const char* filename);

int main (int argc, const char* argv[]) {
  #ifdef DEBUG_BUILD
    for(int i = 0; i < argc; i++)
      logn(argv[i]);
      NEWLINE;
  #endif
  if(argc > 1){
    TIMER_START;
    std::streampos size;
    std::ifstream file (argv[1], std::ios::in|std::ios::binary|std::ios::ate);
    char* filedata;
    if (file.is_open()){
      size = file.tellg();
      filedata = new char [size];
      file.seekg (0, std::ios::beg);
      file.read (filedata, size);
      file.close();
      if(checkFileType(filedata)){
        filedata = filedata + (16 * sizeof(char));
        VM vm(filedata, (uint)size-16);
        vm.run();
        TIMER_STOP;
        std::cin.get();
        return 0;
      }else
        ERR("FILE_FORMAT_NOT_RECOGNIZED");
    }else
      ERR("UNABLE_TO_OPEN_FILE");
  }else
    ERR("NO_FILE_PROVIDED");
}

inline bool checkFileType(char* data){
  return memcmp("BrandonKirincich", data, 16) == 0;
}

// inline char* appendExtension(const char* filename){
//   char* result = (char*) malloc(sizeof(filename)+5);
//   memcpy(result, filename, sizeof(filename));
//   memcpy(&result[sizeof(filename)-1], ".arcb", 5);
//   return result;
// }