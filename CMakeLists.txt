cmake_minimum_required(VERSION 3.0.0)
project(Arcane VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)


SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ARCVM)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/src/Compiler.cpp
)


add_executable(ARCC ${SRC})

target_compile_definitions(ARCC PRIVATE DEBUG_BUILD)

target_include_directories(ARCC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/include PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/lib)

target_compile_options(ARCC PRIVATE "-Wall")

add_executable(ARCC_RELEASE ${SRC})

target_compile_definitions(ARCC_RELEASE PRIVATE RELEASE_BUILD)

target_include_directories(ARCC_RELEASE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/include PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/lib)

target_compile_options(ARCC_RELEASE PRIVATE "-O3")
