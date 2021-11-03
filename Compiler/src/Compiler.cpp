#include "Compiler.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

Compiler::Compiler() {
    PROFILE();
    /* Nothing to see here */
}

arcvm::Arcvm Compiler::compile() {
    PROFILE();
    std::vector<RawFile> projectFiles = get_project_files();

    Parser parser(projectFiles);
    Project *ast = parser.parse();

    TypeInference ti(ast);
    ti.start();

    IRGen gen;
    return gen.gen_project(ast);
}

std::vector<RawFile> Compiler::get_project_files() {   // TODO refactor big time
    PROFILE();
    std::vector<RawFile> result;
    std::vector<std::string> projectFileNames;
    if(!args.project) {
        //TODO find an easy way to get file name
        result.push_back(RawFile{args.path, args.path, read_file(args.path)});
    }
    else {
        for(const auto& file : std::filesystem::directory_iterator(args.path)) {
            if(file.path().filename().string() == "arproj") {
                std::string path = file.path().string();
                projectFileNames = parse_project_spec_file(path);
            }
        }
        for(const auto& file : std::filesystem::directory_iterator(args.path)) {
            for(std::string& name : projectFileNames) {
                if(file.path().filename().string() == name) {
                    result.push_back(RawFile{
                        file.path().string(),
                        file.path().filename().string(),
                        read_file(file.path().string())
                    });
                    break;
                }
            }
        }

    }
    return result;
}

std::vector<std::string> Compiler::parse_project_spec_file(const std::string& filepath) {
    PROFILE();
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

// FIXME this uses the std allocator
// FIXME redundant copies
[[nodiscard]] std::string Compiler::read_file(const std::string& filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return std::string(buffer.str());
}

void Compiler::trim(std::string& str) {  //TODO move this function to a different file
    PROFILE();
    while(!str.empty() && isspace(str.back()))
        str.erase(str.end()-1);
    while(!str.empty() && isspace(str.front()))
        str.erase(str.begin());
}