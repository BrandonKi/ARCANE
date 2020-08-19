#include "MNEMONICS.h"
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

class OS{

    public: 
        static void init();
        static bool EnableVTMode();
};