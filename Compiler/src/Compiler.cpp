#include "Compiler.h"

using namespace pLog;

Compiler::Compiler(){
    
    print(readFile(args.filepath));
}

void Compiler::compile(){

}

/**
 * @brief read the file and return the contents as a string
 * 
 * @param filepath path to file
 * @return [std::string] contents of file
 */
std::string Compiler::readFile(std::string& filepath){
    std::ifstream file;
    file.open(args.filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return std::move(buffer.str());
}