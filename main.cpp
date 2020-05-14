#include <iostream>
// #include <sys/mman.h>
#include <memoryapi.h>

// Allocates RWX memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* alloc_memory(size_t size) {
  // void* ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);     // linux
  void* ptr = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);               // windows
  if (ptr == (void*)-1) {
    std::cerr << "ALLOC_ERROR";
    return NULL;
  }
  return ptr;
}

void emit_code_into_memory(void* m) {
  // unsigned char code[] = {
  //   0x48, 0x89, 0xf8,                   // mov rax, rdi
  //   0x48, 0x83, 0xc0, 0x04,             // add rax, 4          // linux
  //   0xc3                                // ret
  // };
  unsigned char code[] = {
    0x48, 0x89, 0xc8,                   // mov rax, rcx
    0x48, 0x83, 0xc0, 0x08,             // add rax, 4            // windows
    0xc3                                // ret
  };
  memcpy(m, code, sizeof(code));
}

void make_code_executable(void* buf){

    // mprotect(buf, sizeof(*buf), PROT_READ | PROT_EXEC);            // linux
    
    DWORD old;
    VirtualProtect(buf, sizeof(*(char*)buf), PAGE_EXECUTE_READ, &old);       // windows
}

const size_t SIZE = 1024;
typedef long (*JittedFunc)(long);

// Allocates RWX memory directly.
int main() {
  void* m = alloc_memory(SIZE);
  emit_code_into_memory(m);
  make_code_executable(m);

  JittedFunc func = (JittedFunc)m;
  int result = func(2);
  printf("result = %d\n", result);
  std::cin.get();
}
