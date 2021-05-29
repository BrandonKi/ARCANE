#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "Common.h"

enum Severity{ FATAL, WARN, NOTE, MESSAGE };

struct ErrorMessage { 
    Severity severity;
    SourcePos src_pos;
    astring filename;
    astring message;
};

class ErrorHandler {

    public:

        ErrorHandler();
        ~ErrorHandler();

        void exit(ErrorMessage);
        void push(ErrorMessage);
        void flush();

    private:

        astring buffer;

        astring make_preamble(ErrorMessage& error);
        astring create_graphic(ErrorMessage& error);

        std::vector<astring, arena_allocator<astring>> read_file_by_line(const astring& filepath);

    };

#endif