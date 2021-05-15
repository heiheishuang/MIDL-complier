cmake_minimum_required(VERSION 3.19)
project(MIDL_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ../MIDL-scanner
        utils/
        ./
)

add_executable(MIDL_parser
        main.cpp
        SyntaxTreeNode.h
        SyntaxTreeNode.cpp
        Parser.h
        Parser.cpp
        ../MIDL-scanner/Scanner.h
        ../MIDL-scanner/Scanner.cpp
        ../MIDL-scanner/utils/ErrorMsgBuilder.cpp
        ../MIDL-scanner/utils/ErrorMsgBuilder.h
        ../MIDL-scanner/utils/Keyword.cpp
        ../MIDL-scanner/utils/Keyword.h)