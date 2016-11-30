cmake_minimum_required(VERSION 3.6)
project(mypas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    #flthack.c  # just a debug code
    keywords.c
    keywords.h
    lexer.c
    lexer.h
        macros.h
    mypas.c
    mypas.h
    parser.c
    parser.h
    pseudoassembly.c
    pseudoassembly.h
    symtab.c
    symtab.h
    tokens.h)

# add the root folder to the search path for include files
include_directories(".")

add_executable(mypas ${SOURCE_FILES})