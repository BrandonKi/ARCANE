#ifndef COMPILER_H
#define COMPILER_H

#include "Parser.h"

class Compiler final {

    public:
        Compiler();
        ~Compiler() = default;
        std::vector<u8, arena_allocator<u8>> compile();

    private:
        astring read_file(const astring&);

        std::vector<RawFile, arena_allocator<RawFile>> get_project_files();
        std::vector<astring, arena_allocator<astring>> parse_project_spec_file(const astring&);

        void trim(astring&);

};

#endif