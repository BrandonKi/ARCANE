#include "Parser.h"

Parser::Parser(std::vector<RawFile>& projectFiles){  //TODO good candidate for multithreading

    // std::thread worker([](){});
    for(RawFile rf: projectFiles){
        std::cout << rf.filepath << '\n';
        Lexer lexer(rf.filedata);
        data.push_back(LexedFile{rf.filepath, lexer.lex()});
    }
}

Project* Parser::parse(){
    return parseProject();
}

Project* Parser::parseProject(){    //TODO another good candidate for multithreading

    for(LexedFile& lf : data){
        parseFile(lf.filedata);
    }
    return new Project{};
}

File* parseFile(std::vector<Token*>& tokens){
    return new File{};
}