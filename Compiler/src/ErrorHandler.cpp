#include "ErrorHandler.h"


ErrorHandler::ErrorHandler():
    buffer()
{
    buffer.reserve(200);
}

ErrorHandler::~ErrorHandler() {
    flush();
}

void ErrorHandler::push(ErrorMessage error) {
    PROFILE();
    buffer += make_preamble(error);

    switch(error.severity){
        case FATAL:
            buffer += fmt("ERROR ", RED); 
            break;
        case WARN:
            buffer += fmt("WARNING ", YELLOW); 
            break;
        case NOTE:
            buffer += fmt("NOTE ", BLUE); 
            break;
        case MESSAGE:
            buffer += fmt("MESSAGE ", GREEN); 
            break;
    }

    buffer += error.message;
    //TODO output an arrow pointing to the token that caused the error on the next line
}

astring ErrorHandler::make_preamble(ErrorMessage& error){
    PROFILE();
    return error.filename + '[' + to_astring(error.token->pos.src_line) + ", " + to_astring(error.token->pos.src_char) + "] : ";
}

void ErrorHandler::flush(){
    PROFILE();
    if(!buffer.empty())
        println(astrtostr(buffer));
    buffer.clear();
}