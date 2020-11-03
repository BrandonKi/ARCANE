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
        ErrorHandler(const std::string&);

        void log(ErrorMessage);
        void flush();

    private:

        std::string buffer;
        const std::string& src;

        std::string makePreamble(ErrorMessage& error);

};