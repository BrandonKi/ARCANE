//TODO include guards for all files
#include "Common.h"

enum Severity{ FATAL, WARN, NOTE, MESSAGE };

struct ErrorMessage{ 
    Severity severity;
    Token* token;
    std::string filename;
    std::string message;
};

class ErrorHandler {

    public:

        ErrorHandler();
        ErrorHandler(std::string&);

        void log(ErrorMessage&);
        std::string makePreamble(ErrorMessage& error);
        void flush();

    private:

        std::string buffer;
        std::string filepath;


};