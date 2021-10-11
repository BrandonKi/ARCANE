#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "Common.h"

enum Severity{ FATAL, WARN, NOTE, MESSAGE };

struct ErrorMessage { 
    Severity severity;
    SourcePos src_pos;
    std::string filename;
    std::string message;
};

class ErrorHandler {

    public:

        ErrorHandler();
        ~ErrorHandler();

        void exit(ErrorMessage);
        void push(ErrorMessage);
        void flush();

    private:

        std::string buffer;

        std::string make_preamble(ErrorMessage& error);
        std::string create_graphic(ErrorMessage& error);

        std::vector<std::string> read_file_by_line(const std::string& filepath);

    };

#endif