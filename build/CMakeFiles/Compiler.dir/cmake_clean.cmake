file(REMOVE_RECURSE
  "../bin/Compiler.exe"
  "../bin/Compiler.exe.manifest"
  "../bin/Compiler.pdb"
  "../bin/libCompiler.dll.a"
  "CMakeFiles/Compiler.dir/Compiler/src/Compiler.cpp.obj"
  "CMakeFiles/Compiler.dir/Compiler/src/ErrorHandler.cpp.obj"
  "CMakeFiles/Compiler.dir/Compiler/src/Lexer.cpp.obj"
  "CMakeFiles/Compiler.dir/Compiler/src/OS.cpp.obj"
  "CMakeFiles/Compiler.dir/Compiler/src/Parser.cpp.obj"
  "CMakeFiles/Compiler.dir/Compiler/src/SymbolTable.cpp.obj"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/Compiler.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
