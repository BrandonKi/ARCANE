#include "ErrorHandler.h"


ErrorHandler::ErrorHandler():
    buffer()
{
    buffer.reserve(200);
}

ErrorHandler::ErrorHandler(std::string& filepath):
    buffer(), filepath(filepath)
{
    buffer.reserve(200);
}

void ErrorHandler::log(ErrorMessage& error) {

    buffer += makePreamble(error);

    switch(error.severity){
        case FATAL:
            buffer += fmt("ERROR "); 
            break;
        case WARN:
            break;
        case NOTE:
            break;
        case MESSAGE:
            break;

    }
}

std::string ErrorHandler::makePreamble(ErrorMessage& error){
    return std::move(error.filename + '[' + std::to_string(error.token->srcLine) + ", " + std::to_string(error.token->srcChar) + "] : ");
}

void ErrorHandler::flush(){
    if(filepath.size() == 0){
        println(buffer);
    }
    else{
        std::ofstream file(filepath);
        file << buffer;
    }
}