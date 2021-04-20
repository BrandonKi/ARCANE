cmake_minimum_required(VERSION 3.0.0)
project(Arcane VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0069 NEW)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ARCVM-V2)

set(arcvm_include ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM-V2/include)
set(arcvm_source
    ${CMAKE_CURRENT_SOURCE_DIR}/ARCVM-V2/src/ARCVM.cpp
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
  target_compile_options(ARCC PRIVATE /W4 /WX)
else()
  target_compile_options(ARCC PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# target_compile_options(ARCC PRIVATE "-W" "-Wall" "-Wextra" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic" "-Wno-newline-eof" "-Wno-exit-time-destructors" "-Wno-header-hygiene" "-Wno-global-constructors" "-Wno-nonportable-system-include-path")
