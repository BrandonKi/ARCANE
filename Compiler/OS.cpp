#include "OS.h"

void OS::init(){
    #ifdef _WIN32
        EnableVTMode();
    #endif
}


bool OS::EnableVTMode()
{
    #ifdef _WIN32
        // Set output mode to handle virtual terminal sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
        {
            return false;
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode))
        {
            return false;
        }
        return true;
    #endif
    return false;
}
