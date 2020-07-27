#include <fstream>
#include <chrono> 
#include "VM.h"

std::chrono::high_resolution_clock::time_point start, end; 
#define TIMER_START start = std::chrono::high_resolution_clock::now()
#define TIMER_STOP end =  std::chrono::high_resolution_clock::now(); \
                   std::chrono::duration<int64_t,std::nano> time_span = std::chrono::duration<int64_t,std::nano>(end - start); \
                   std::cout << "Time taken by program is : " << (time_span.count()/1000000) << " ms " << std::endl; 

inline bool checkFileType(char* data);
inline char* appendExtension(const char* filename);

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
        #ifdef DEBUG_BUILD
        std::cin.get();
        #endif
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

inline char* appendExtension(const char* filename){
  char* result = (char*) malloc(sizeof(filename)+5);
  memcpy(result, filename, sizeof(filename));
  memcpy(&result[sizeof(filename)-1], ".arcb", 5);
  return result;
}