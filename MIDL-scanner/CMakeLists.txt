cmake_minimum_required(VERSION 3.14)
project(MIDL_scanner)

set(CMAKE_CXX_STANDARD 14)

add_executable(MIDL_scanner main.cpp
        Scanner.cpp
        Scanner.h
        utils/Keyword.cpp
        utils/Keyword.h
        utils/TokenType.h
        utils/ErrorMsgBuilder.cpp
        utils/ErrorMsgBuilder.h DFA.h)