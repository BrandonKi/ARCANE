#include "Compiler.h"

Compiler::Compiler() {
    PROFILE();
    /* Nothing to see here */
}

std::vector<u8, arena_allocator<u8>> Compiler::compile() {
    PROFILE();
    std::vector<RawFile, arena_allocator<RawFile>> projectFiles = get_project_files();
    Parser parser(projectFiles);
    Project *ast = parser.parse();
    
    //TODO convert to another IR for easier optimization and optimize step

    BytecodeGen gen(ast);
    return gen.gen_code();
}

astring Compiler::read_file(astring filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return astring(buffer.str());
}

std::vector<RawFile, arena_allocator<RawFile>> Compiler::get_project_files() {   // TODO refactor big time
    PROFILE();
    std::vector<RawFile, arena_allocator<RawFile>> result;
    std::vector<astring, arena_allocator<astring>> projectFileNames;
    if(!args.project){
        result.push_back(RawFile{args.path, read_file(args.path)});
    }
    else{
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            if(file.path().filename().string() == "ARProjSpec"){
                astring path = strtoastr(file.path().string());
                projectFileNames = parse_project_spec_file(path);
            }
        }
        for(const auto& file : std::filesystem::directory_iterator(args.path)){
            for(astring& name : projectFileNames){
                if(strtoastr(file.path().filename().string()) == name){
                    result.push_back(RawFile{strtoastr(file.path().string()), read_file(strtoastr(file.path().string()))});
                    break;
                }
            }
        }

    }
    return result;
}

std::vector<astring, arena_allocator<astring>> Compiler::parse_project_spec_file(astring& filepath) {
    PROFILE();
    std::vector<astring, arena_allocator<astring>> result;
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