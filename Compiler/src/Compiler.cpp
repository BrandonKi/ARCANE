#include "Compiler.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

Compiler::Compiler() {
    PROFILE();
    /* Nothing to see here */
}

std::vector<u8, arena_allocator<u8>> Compiler::compile() {
    PROFILE();
    std::vector<RawFile, arena_allocator<RawFile>> projectFiles = get_project_files();
    //TODO do some heuristics to pick a size for the arena
    auto size = projectFiles[0].filedata.size() * 100;
    set_arena_alloc_size(size);
    
    Parser parser(projectFiles);
    Project *ast = parser.parse();

    TypeInference ti(ast);
    ti.start();
    
    //TODO convert to another IR for easier optimization and optimize step

    BytecodeGen gen(ast);
    return gen.gen_code();
}

std::vector<RawFile, arena_allocator<RawFile>> Compiler::get_project_files() {   // TODO refactor big time
    PROFILE();
    std::vector<RawFile, arena_allocator<RawFile>> result;
    std::vector<astring, arena_allocator<astring>> projectFileNames;
    if(!args.project) {
        //TODO find an easy way to get file name
        result.push_back(RawFile{args.path, args.path, read_file(args.path)});
    }
    else {
        for(const auto& file : std::filesystem::directory_iterator(args.path)) {
            if(file.path().filename().string() == "arproj") {
                astring path = strtoastr(file.path().string());
                projectFileNames = parse_project_spec_file(path);
            }
        }
        for(const auto& file : std::filesystem::directory_iterator(args.path)) {
            for(astring& name : projectFileNames) {
                if(strtoastr(file.path().filename().string()) == name) {
                    result.push_back(RawFile{
                        strtoastr(file.path().string()),
                        strtoastr(file.path().filename().string()),
                        read_file(strtoastr(file.path().string()))
                    });
                    break;
                }
            }
        }

    }
    return result;
}

std::vector<astring, arena_allocator<astring>> Compiler::parse_project_spec_file(const astring& filepath) {
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

// FIXME this uses the std allocator
// FIXME redundant copies
[[nodiscard]] astring Compiler::read_file(const astring& filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return astring(buffer.str());
}

void Compiler::trim(astring& str) {  //TODO move this function to a different file
    PROFILE();
    while(!str.empty() && isspace(str.back()))
        str.erase(str.end()-1);
    while(!str.empty() && isspace(str.front()))
        str.erase(str.begin());
}