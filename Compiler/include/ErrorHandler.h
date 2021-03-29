//TODO include guards for all files
#include "Common.h"

enum Severity{ FATAL, WARN, NOTE, MESSAGE };

struct ErrorMessage { 
    Severity severity;
    Token* token;
    std::string filename;
    std::string message;
};

class ErrorHandler {

    public:

        ErrorHandler();

        void push(ErrorMessage);
        void flush();

    private:

        std::string buffer;

        std::string makePreamble(ErrorMessage& error);

};
