#include "Token.h"



class ErrorHandler{

    public:

        inline static const char* m_filedata; // must be inline or define the variable in the cpp file

        static void init(const char* f_data);

        static void printError(ErrorType, std::vector<Token*>&, unsigned int);

    private:

        inline static std::string F_BLACK(std::string);
        inline static std::string F_DULL_BLACK(std::string);
        inline static std::string F_RED(std::string);
        inline static std::string F_DULL_RED(std::string);
        inline static std::string F_GREEN(std::string);
        inline static std::string F_DULL_GREEN(std::string);
        inline static std::string F_YELLOW(std::string);
        inline static std::string F_DULL_YELLOW(std::string);
        inline static std::string F_BLUE(std::string);
        inline static std::string F_DULL_BLUE(std::string);
        inline static std::string F_PINK(std::string);
        inline static std::string F_DULL_PINK(std::string);
        inline static std::string F_CYAN(std::string);
        inline static std::string F_DULL_CYAN(std::string);
        inline static std::string F_WHITE(std::string);
        inline static std::string F_DULL_WHITE(std::string);
};