#include <iostream>
#include <cstring>
// #include <sys/mman.h>
#include <memoryapi.h>

// Allocates RW memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* allocMemory(size_t size) {
  // void* ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);     // linux
  void* ptr = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);               // windows
  if (ptr == (void*)-1) {
    std::cerr << "ALLOC_ERROR";
    return NULL;
  }
  return ptr;
}

void emit(void* m, unsigned char* code) {
  
  
  memcpy(m, code, sizeof(code));
}

void* makeExecutable(void* buf){

    // mprotect(buf, sizeof(*(char*)buf), PROT_READ | PROT_EXEC);            // linux
    
    DWORD old;
    VirtualProtect(buf, sizeof(*(char*)buf), PAGE_EXECUTE_READ, &old);       // windows

    return buf;
}

const size_t SIZE = 1024;
typedef long (*exe)(long, long);
 

int main() {
  //  unsigned char code[] = {
  //    0x48, 0x89, 0xf8,                   // mov rax, rdi
  //    0x48, 0x83, 0xc0, 0x08,             // add rax, 4          // linux
  //    0xc3                                // ret
  //  };
  // unsigned char code[] = { 
  //   0x48, 0x01, 0xD1,                       // add rcx, rdx
  //   0x48, 0x89, 0xC8,                       // mov rax, rcx
  //   0xC3                                    // ret
  // };

  unsigned char code[] = {
    0x4C, 0x89, 0xC0,
    0x48, 0x01, 0xC0,
    0x48, 0x39, 0xD1,
    0x48, 0x83, 0xC1, 0x01,
    0x75, 0xF4, 0xC3
    };
  void* m = allocMemory(SIZE);
  emit(m, code);
  exe func = (exe)makeExecutable(m);
  int temp = 10;
  int result = func(2, temp);
  printf("result = %d\n", result);
  std::cin.get();
}


// int i = 1;
//for(1 -> 10)
//  i += i;
