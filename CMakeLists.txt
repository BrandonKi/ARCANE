cmake_minimum_required(VERSION 3.0.0)
project(Arcane VERSION 0.1.0)

include(CTest)

set (CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

### Compiler ###

set(C_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src)

set(COMPILER_SOURCES 
    ${C_SRC_DIR}/Compiler.cpp 
    ${C_SRC_DIR}/ErrorHandler.cpp 
    ${C_SRC_DIR}/Lexer.cpp 
    ${C_SRC_DIR}/OS.cpp 
    ${C_SRC_DIR}/Parser.cpp 
    ${C_SRC_DIR}/SymbolTable.cpp
)

add_executable(Compiler ${COMPILER_SOURCES})

target_include_directories(Compiler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/include)

### ARCVM ###

set(VM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/src/BMain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/src/VM.cpp)

add_executable(ARCVM ${VM_SOURCES})

target_include_directories(ARCVM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/include)

### Filewriter ###

set(FW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/debug/filewriter.cpp)

add_executable(filewriter ${FW_SOURCES})

### Disassemble ###

set(DA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/debug/Disassemble.cpp)

add_executable(Disassembler ${DA_SOURCES})