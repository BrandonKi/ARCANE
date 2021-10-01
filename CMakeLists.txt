cmake_minimum_required(VERSION 3.0.0)
project(Arcane VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0069 NEW)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ARCVM)

set(arcvm_include ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/include)
set(arcvm_source
    ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/src/IRGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/src/IRInterpreter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM/src/IRPrinter.cpp
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/AST.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/ErrorHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/SymbolTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/BytecodeGen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/BytecodeLinker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/TypeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/TypeInference.cpp
    ${arcvm_source}
)

add_executable(ARCC ${SRC})

target_compile_definitions(ARCC PRIVATE DEBUG_BUILD)

target_include_directories(ARCC 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/lib
    PRIVATE ${arcvm_include}
)
# hmmm keep an eye on this
if(${LTO})
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

if(MSVC)
    target_compile_options(ARCC PRIVATE 
        /W3 
        # /Wall 
        # # # C5045 Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        # /wd5045
        # # # C5039 'function': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
        # /wd5039
        # # # C4668 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
        # /wd4668

        # /wd5027
        # /wd4626
        # /wd4820
        # /wd4061
        # /wd4062
        # /wd4623
    )
else()
    target_compile_options(ARCC PRIVATE -Wall -Wextra -pedantic)
endif()