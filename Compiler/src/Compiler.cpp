#include "Compiler.h"

Compiler::Compiler() {
    PROFILE();
    /* Nothing to see here */
}

std::vector<u8> Compiler::compile() {
    PROFILE();
    std::vector<RawFile> projectFiles = getProjectFiles();
    Parser parser(projectFiles);
    parser.parse();
    
    //TODO convert to another IR for easier optimization and optimize step

    // CodeGenerator gen;
    // gen.generate(/* Whatever IR I decide on */); 

    
    std::vector<u8> temp;        //TODO replace in the future this is just so the code compiles and runs prerequisites
    return temp;
}

astring Compiler::readFile(astring filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return astring(buffer.str());
}

std::vector<RawFile> Compiler::getProjectFiles() {   // TODO refactor big time
    PROFILE();
    std::vector<RawFile> result;
    std::vector<astring> projectFileNames;
    if(!args.project){
        result.push_back(RawFile{args.path, readFile(args.path)});
    }
    else{
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            if(file.path().filename().string() == "ARProjSpec"){
                astring path = strtoastr(file.path().string());
                projectFileNames = parseProjectSpecFile(path);
            }
        }
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            for(astring& name : projectFileNames){
                if(strtoastr(file.path().filename().string()) == name){
                    result.push_back(RawFile{strtoastr(file.path().string()), readFile(strtoastr(file.path().string()))});
                    break;
                }
            }
        }

    }
    return result;
}

std::vector<astring> Compiler::parseProjectSpecFile(astring& filepath) {
    PROFILE();
    std::vector<astring> result;
    std::ifstream file(filepath);
    astring line;
    while(std::getline(file, line)){
        trim(line);
        if(line[0] != '#')
            result.push_back(line);
    }
    return result;
}

void Compiler::trim(astring& str) {  //TODO move this function to a different file
    PROFILE();
    while(!str.empty() && isspace(str.back()))
        str.erase(str.end()-1);
    while(!str.empty() && isspace(str.front()))
        str.erase(str.begin());
}