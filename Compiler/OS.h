#ifdef _WIN32
    #include <windows.h>
#endif

class OS{

    public:
        static void init();
        static bool EnableVTMode();
};