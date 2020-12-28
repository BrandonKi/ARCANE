#include "ErrorHandler.h"


ErrorHandler::ErrorHandler():
    buffer()
{
    buffer.reserve(200);
}

void ErrorHandler::push(ErrorMessage error) {

    buffer += makePreamble(error);

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

std::string ErrorHandler::makePreamble(ErrorMessage& error){
    return error.filename + '[' + std::to_string(error.token->pos.srcLine) + ", " + std::to_string(error.token->pos.srcChar) + "] : ";
}

void ErrorHandler::flush(){
    println(buffer);
}