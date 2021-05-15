//
// Created by Zou on 4/18/21.
//

#ifndef MIDL_SCANNER_TOKENTYPE_H
#define MIDL_SCANNER_TOKENTYPE_H

enum class TokenType {
    STRUCT = 0,
    FLOAT,
    BOOLEAN,
    SHORT,
    LONG,
    DOUBLE,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    CHAR,
    UNSIGNED,
    L_BRACE,
    R_BRACE,
    SEMICOLON,
    L_PARENTHESES,
    R_PARENTHESES,
    MULTIPLY,
    PLUS,
    MINUS,
    TILDE,
    DIVISION,
    PERCENT,
    AND,
    CARET,
    OR,
    L_BRACKET,
    R_BRACKET,
    COMMA,
    R_GUILLEMET,
    L_GUILLEMET,

    STRING,
    ID,
    INTEGER,

    EOF_TOKEN,
    NULL_TOKEN,
    ERROR,
    SPACE,
    TAB,
    ENTER
};

#endif //MIDL_SCANNER_TOKENTYPE_H
