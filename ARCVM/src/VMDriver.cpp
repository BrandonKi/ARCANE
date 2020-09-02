#include <fstream>
#include <chrono>
#include <iomanip>
#include "VM.h"

#ifdef _WIN32
  #include <windows.h>

  LARGE_INTEGER start, end, frequency;

  #define TIMER_START QueryPerformanceFrequency(&frequency); \
                      QueryPerformanceCounter(&start);

  #define TIMER_STOP  QueryPerformanceCounter(&end);

  #define TIMER_PRINT std::cout << std::fixed << std::setprecision(6) << \
                      "Time taken : " << ((((end.QuadPart - start.QuadPart) * 1000000)/frequency.QuadPart)/1000000.0) << " seconds \n"; 

#elif defined(__unix__)
  #include<time.h>
  timespec start, end;
  #define TIMER_START clock_gettime(CLOCK_REALTIME, &start);
  
  #define TIMER_STOP clock_gettime(CLOCK_REALTIME, &end);

  #define TIMER_PRINT std::cout << "Time taken: " << \
                      (compute_cpu_time(start, end)/ 1E9) \
                      << " seconds \n"

  double compute_cpu_time(timespec start, timespec end){
    double nano = static_cast<double>(end.tv_nsec - start.tv_nsec);
    double sec = static_cast<double>((end.tv_sec - start.tv_sec) * 1E9);

    return sec + nano;
  }

#else
  std::chrono::high_resolution_clock::time_point start, end; 
  #define TIMER_START start = std::chrono::high_resolution_clock::now()
  #define TIMER_STOP end =  std::chrono::high_resolution_clock::now();
  #define TIMER_PRINT auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
                      std::cout << std::fixed << std::setprecision(6) << "Time taken : " << (time_span.count()/1000.0) << " seconds \n";  
#endif

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
    char* filename = appendExtension(argv[1]);
    std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
    char* filedata;
    if (file.is_open()){
      size = file.tellg();
      filedata = new char [size];
      file.seekg (0, std::ios::beg);
      file.read (filedata, size);
      file.close();
      if(checkFileType(filedata)){
        filedata = filedata + (16 * sizeof(char)); // causes a mem leak, but it doesn't matter too much
        VM vm(filedata, (u64)size-16);
        vm.run();
        TIMER_STOP;
        TIMER_PRINT;
        #ifdef DEBUG_BUILD
        delete[] (filedata-16);
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
  char* result = (char*) malloc(strlen(filename)+5);
  memcpy(result, filename, strlen(filename));
  memcpy(result + strlen(filename), ".arcb\0", 6);
  return result;
}