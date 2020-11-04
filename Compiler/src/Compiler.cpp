#include "Compiler.h"

/**
 * @brief Construct a Compiler object
 * also initializes data by reading from a file
 */
Compiler::Compiler():
    data(readFile(args.filepath))
{
    /* Nothing to see here */
}

std::vector<u8> Compiler::compile(const std::string& code) {
    Parser parser(code);
    parser.parse();

    //TODO convert to another IR for easier optimization
    //TODO optimize step

    // CodeGenerator gen;
    // gen.generate(/* Whatever IR I decide on */);

    
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
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}


/**
 * @brief return a string of the current file data
 * 
 * @return [std::string] a copy of the current file data
 */
std::string Compiler::filedata(){
    return data;
}