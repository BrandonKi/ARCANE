#include "Token.h"



class ErrorHandler{

    public:

        inline static const char* m_filedata; // must be inline or define the variable in the cpp file

        static void init(const char* f_data);

        static void printError(ErrorType, std::vector<Token*>&, unsigned int);

    private:

        static std::string F_RED(std::string);
        static std::string F_DULL_RED(std::string);
        static std::string F_YELLOW(std::string);
        static std::string F_DULL_YELLOW(std::string);
        static std::string F_BLUE(std::string);
        static std::string F_DULL_BLUE(std::string);
};