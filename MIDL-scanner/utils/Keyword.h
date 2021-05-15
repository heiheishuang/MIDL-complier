//
// Created by Zou on 4/18/21.
//

#ifndef MIDL_SCANNER_KEYWORD_H
#define MIDL_SCANNER_KEYWORD_H

#include <string>
#include <unordered_map>
#include "TokenType.h"

class Keyword {
public:
    Keyword();
    virtual ~Keyword();
    static bool IsBlankChar(const char &input_char);
    bool IsKeyWords(const std::string &input_string, std::string &token);
    static bool IsLetterOrDigit(const char &input_char);
    static bool IsBoolean(const std::string &input_string, std::string &token);
    static bool IsDigit(const char &input_char);
    static bool IsDigitButNotZero(const char &input_char);
    static bool IsExcapeSequence(const char &input_char);
    static bool IsSlant(const char &input_char);
    static bool IsEndOfFile(const char &input_char);
    static bool IsStringAllow(const char &input_char);
    static bool IsZero(const char &input_char);
    static bool IsL(const char &input_char);
    static bool IsQuotation(const char &input_char);
    static bool IsLetter(const char &input_char);
    static bool IsUnderLine(const char &input_char);
    static bool IsLowerSignal(const char &input_char);
    static bool IsUpperSignal(const char &input_char);
    bool IsSingleCharOperator(const char &input_char, std::string &token);
    bool IsDoubleCharOperatorFirst(const char &input_char);
    bool IsDoubleCharOperatorSecond(const char &input_char, const char &first_char, std::string &token);

    static TokenType StringToTokenType(const std::string& token_type);
    static std::string TokenTypeToString(const TokenType& token_type);

private:
    std::unordered_map<std::string, std::string> umap_keywords_;
    std::unordered_map<char, std::string> umap_single_char_operators_;
    std::unordered_map<std::string, std::string> umap_double_char_operators_;
};


#endif //MIDL_SCANNER_KEYWORD_H
