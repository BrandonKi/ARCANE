#ifndef COMPILER_H
#define COMPILER_H

#include "Parser.h"

#include <Arcvm.h>

class Compiler final {

    public:
        Compiler();
        ~Compiler() = default;
        arcvm::Arcvm compile();

    private:
        std::string read_file(const std::string&);

        std::vector<RawFile, arena_allocator<RawFile>> get_project_files();
        std::vector<std::string, arena_allocator<std::string>> parse_project_spec_file(const std::string&);

        void trim(std::string&);

};

#endif