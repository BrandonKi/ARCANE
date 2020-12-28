#include "Compiler.h"

Compiler::Compiler(){
    /* Nothing to see here */
}

std::vector<u8> Compiler::compile() {
    Parser parser(getProjectFiles());
    parser.parse();
    
    //TODO convert to another IR for easier optimization and optimize step

    // CodeGenerator gen;
    // gen.generate(/* Whatever IR I decide on */);

    
    std::vector<u8> temp;        //TODO replace in the future this is just so the code compiles and runs prerequisites
    return temp;
}

std::string Compiler::readFile(const std::string& filepath){
    std::ifstream file;
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::vector<RawFile> Compiler::getProjectFiles(){   // TODO refactor big time
    std::vector<RawFile> result;
    std::vector<std::string> projectFileNames;
    if(!args.project){
        result.push_back(RawFile{args.path, readFile(args.path)});
    }
    else{
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            if(file.path().filename().string() == "ARProjSpec"){
                projectFileNames = parseProjectSpecFile(file.path().string());
            }
        }
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            for(std::string& name : projectFileNames){
                if(file.path().filename().string() == name){
                    result.push_back(RawFile{file.path().string(), readFile(file.path().string())});
                    break;
                }
            }
        }

    }
    return result;
}

std::vector<std::string> Compiler::parseProjectSpecFile(std::string& filepath){
    std::vector<std::string> result;
    std::ifstream file(filepath);
    std::string line;
    while(std::getline(file, line)){
        trim(line);
        if(line[0] != '#')
            result.push_back(line);
    }
    return result;
}

void Compiler::trim(std::string& str){  //TODO move this function to a different file
    while(!str.empty() && isspace(str.back()))
        str.erase(str.end()-1);
    while(!str.empty() && isspace(str.front()))
        str.erase(str.begin());
}