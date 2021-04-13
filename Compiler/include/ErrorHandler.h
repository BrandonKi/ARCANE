#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "Common.h"

enum Severity{ FATAL, WARN, NOTE, MESSAGE };

struct ErrorMessage { 
    Severity severity;
    Token* token;
    astring filename;
    astring message;
};

class ErrorHandler {

    public:

        ErrorHandler();
        ~ErrorHandler();

        void push(ErrorMessage);
        void flush();

    private:

        astring buffer;

        astring make_preamble(ErrorMessage& error);

};

#endif