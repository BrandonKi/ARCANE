#include "Compiler.h"

using namespace pLog;

Compiler::Compiler(){
    data = std::move(readFile(args.filepath));
}

std::vector<u8> Compiler::compile(const std::string& code){
    // Parser parser;
    // parser.parse(code);

    //TODO convert to another IR for easier optimization
    //TODO optimize step

    // CodeGenerator gen;
    // gen.convert(/* Whatever IR I decide on */);

    
    std::vector<u8> temp;        //TODO replace in the future this is just so the code compiles and runs prerequisites
    return temp;
}

/**
 * @brief read the file and return the contents as a string
 * 
 * @param filepath path to file
 * @return [std::string] contents of file
 */
std::string Compiler::readFile(const std::string& filepath){
    std::ifstream file;
    file.open(args.filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return std::move(buffer.str());
}

std::string Compiler::filedata(){
    return data;
}