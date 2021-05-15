//
// Created by Zou on 4/18/21.
//

#include "Keyword.h"

Keyword::Keyword() {
    /*
     * Keywords
     */
    umap_keywords_.insert({"struct", "STRUCT"});
    umap_keywords_.insert({"float", "FLOAT"});
    umap_keywords_.insert({"boolean", "BOOLEAN"});
    umap_keywords_.insert({"short", "SHORT"});
    umap_keywords_.insert({"long", "LONG"});
    umap_keywords_.insert({"double", "DOUBLE"});
    umap_keywords_.insert({"int8", "INT8"});
    umap_keywords_.insert({"int16", "INT16"});
    umap_keywords_.insert({"int32", "INT32"});
    umap_keywords_.insert({"int64", "INT64"});
    umap_keywords_.insert({"uint8", "UINT8"});
    umap_keywords_.insert({"uint16", "UINT16"});
    umap_keywords_.insert({"uint32", "UINT32"});
    umap_keywords_.insert({"uint64", "UINT64"});
    umap_keywords_.insert({"char", "CHAR"});
    umap_keywords_.insert({"unsigned", "UNSIGNED"});


    /*
     * Single char operators 17
     */
    umap_single_char_operators_.insert({'{', "L_BRACE"});
    umap_single_char_operators_.insert({'}', "R_BRACE"});
    umap_single_char_operators_.insert({';', "SEMICOLON"});
    umap_single_char_operators_.insert({'(', "L_PARENTHESES"});
    umap_single_char_operators_.insert({')', "R_PARENTHESES"});
    umap_single_char_operators_.insert({'*', "MULTIPLY"});
    umap_single_char_operators_.insert({'+', "PLUS"});
    umap_single_char_operators_.insert({'-', "MINUS"});
    umap_single_char_operators_.insert({'~', "TILDE"});
    umap_single_char_operators_.insert({'/', "DIVISION"});
    umap_single_char_operators_.insert({'%', "PERCENT"});
    umap_single_char_operators_.insert({'&', "AND"});
    umap_single_char_operators_.insert({'^', "CARET"});
    umap_single_char_operators_.insert({'|',"OR"});
    umap_single_char_operators_.insert({'[', "L_BRACKET"});
    umap_single_char_operators_.insert({']', "R_BRACKET"});
    umap_single_char_operators_.insert({',', "COMMA"});
    umap_single_char_operators_.insert({'"', "Quotation"});

    /*
     * Double char operators 2
     * >> <<
     */
    umap_double_char_operators_.insert({">>", "R_GUILLEMET"});
    umap_double_char_operators_.insert({"<<", "L_GUILLEMET"});

}

Keyword::~Keyword() = default;

bool Keyword::IsBlankChar(const char &input_char) {
    return input_char == '\t' ||
           input_char == '\r' ||
           input_char == '\n' ||
           input_char == ' ';
}

bool Keyword::IsKeyWords(const std::string &input_string, std::string &token) {
    /*
     * Found key words
     */

    std::string up = input_string;
    std::transform(up.begin(), up.end(), up.begin(), ::tolower);
    if (umap_keywords_.find(up) != umap_keywords_.end()) {
        token = umap_keywords_.at(up);
        return true;
    }
    return false;
}

bool Keyword::IsLetterOrDigit(const char &input_char) {
    return IsLetter(input_char) || IsDigit(input_char);
}

bool Keyword::IsStringAllow(const char &input_char) {
    return input_char != '\\' && input_char != '"' && input_char != '\r';

}

bool Keyword::IsDigit(const char &input_char) {
    return input_char >= '0' && input_char <= '9';
}

bool Keyword::IsZero(const char &input_char) {
    return input_char == '0';
}

bool Keyword::IsDigitButNotZero(const char &input_char) {
    return input_char >= '1' && input_char <= '9';
}

bool Keyword::IsLetter(const char &input_char) {
    return (input_char >= 'a' && input_char <= 'z') ||
           (input_char >= 'A' && input_char <= 'Z');
}

bool Keyword::IsUnderLine(const char &input_char) {
    return input_char == '_';
}

bool Keyword::IsSingleCharOperator(const char &input_char, std::string &token) {
    /*
     * Found unary operator
     */
    if (umap_single_char_operators_.find(input_char) != umap_single_char_operators_.end()) {
        token = umap_single_char_operators_.at(input_char);
        return true;
    }
    return false;
}

bool Keyword::IsDoubleCharOperatorFirst(const char &input_char) {
    for (const auto &double_char_operator : umap_double_char_operators_) {
        if (double_char_operator.first.size() == 2 &&
            double_char_operator.first.at(0) == input_char) {
            return true;
        }
    }
    return false;
}

bool Keyword::IsDoubleCharOperatorSecond(const char &input_char,
                                            const char &first_char,
                                            std::string &token) {
    std::string double_char_operator{first_char, input_char};
    if (umap_double_char_operators_.find(double_char_operator) != umap_double_char_operators_.end()) {
        token = umap_double_char_operators_.at(double_char_operator);
        return true;
    }
    return false;
}

TokenType Keyword::StringToTokenType(const std::string &token_type) {
    /*
     * String to TokenTypes
     */
    if (token_type == "STRUCT") {
        return TokenType::STRUCT;
    } else if (token_type == "FLOAT") {
        return TokenType::FLOAT;
    } else if (token_type == "BOOLEAN") {
        return TokenType::BOOLEAN;
    } else if (token_type == "SHORT") {
        return TokenType::SHORT;
    } else if (token_type == "LONG") {
        return TokenType::LONG;
    } else if (token_type == "DOUBLE") {
        return TokenType::DOUBLE;
    } else if (token_type == "INT8") {
        return TokenType::INT8;
    } else if (token_type == "INT16") {
        return TokenType::INT16;
    } else if (token_type == "INT32") {
        return TokenType::INT32;
    } else if (token_type == "INT64") {
        return TokenType::INT64;
    } else if (token_type == "UINT8") {
        return TokenType::UINT8;
    } else if (token_type == "UINT16") {
        return TokenType::UINT16;
    } else if (token_type == "UINT32") {
        return TokenType::UINT32;
    } else if (token_type == "UINT64") {
        return TokenType::UINT64;
    } else if (token_type == "CHAR") {
        return TokenType::CHAR;
    } else if (token_type == "UNSIGNED") {
        return TokenType::UNSIGNED;
    } else if (token_type == "L_BRACE") {
        return TokenType::L_BRACE;
    } else if (token_type == "R_BRACE") {
        return TokenType::R_BRACE;
    } else if (token_type == "SEMICOLON") {
        return TokenType::SEMICOLON;
    } else if (token_type == "L_PARENTHESES") {
        return TokenType::L_PARENTHESES;
    } else if (token_type == "R_PARENTHESES") {
        return TokenType::R_PARENTHESES;
    } else if (token_type == "MULTIPLY") {
        return TokenType::MULTIPLY;
    } else if (token_type == "PLUS") {
        return TokenType::PLUS;
    } else if (token_type == "MINUS") {
        return TokenType::MINUS;
    } else if (token_type == "TILDE") {
        return TokenType::TILDE;
    } else if (token_type == "DIVISION") {
        return TokenType::DIVISION;
    } else if (token_type == "PERCENT") {
        return TokenType::PERCENT;
    } else if (token_type == "AND") {
        return TokenType::AND;
    } else if (token_type == "CARET") {
        return TokenType::CARET;
    } else if (token_type == "OR") {
        return TokenType::OR;
    } else if (token_type == "L_BRACKET") {
        return TokenType::L_BRACKET;
    } else if (token_type == "R_BRACKET") {
        return TokenType::R_BRACKET;
    } else if (token_type == "COMMA") {
        return TokenType::COMMA;
    } else if (token_type == "R_GUILLEMET") {
        return TokenType::R_GUILLEMET;
    } else if (token_type == "L_GUILLEMET") {
        return TokenType::L_GUILLEMET;
    } else if (token_type == "ID") {
        return TokenType::ID;
    } else if (token_type == "STRING") {
        return TokenType::STRING;
    } else if (token_type == "INTEGER") {
        return TokenType::INTEGER;
    }

    return TokenType::ERROR;
}

std::string Keyword::TokenTypeToString(const TokenType &token_type) {
    /*
      * String to TokenTypes
      */
    if (token_type == TokenType::STRUCT) {
        return "STRUCT";
    } else if (token_type == TokenType::FLOAT) {
        return "FLOAT";
    } else if (token_type == TokenType::BOOLEAN) {
        return "BOOLEAN";
    } else if (token_type == TokenType::SHORT) {
        return "SHORT";
    } else if (token_type == TokenType::LONG) {
        return "LONG";
    } else if (token_type == TokenType::DOUBLE) {
        return "DOUBLE";
    } else if (token_type == TokenType::INT8) {
        return "INT8";
    } else if (token_type == TokenType::INT16) {
        return "INT16";
    } else if (token_type == TokenType::INT32) {
        return "INT32";
    } else if (token_type == TokenType::INT64) {
        return "INT64";
    } else if (token_type == TokenType::UINT8) {
        return "UINT8";
    } else if (token_type == TokenType::UINT16) {
        return "UINT16";
    } else if (token_type == TokenType::UINT32) {
        return "UINT32";
    } else if (token_type == TokenType::UINT64) {
        return "UINT64";
    } else if (token_type == TokenType::CHAR) {
        return "CHAR";
    } else if (token_type == TokenType::UNSIGNED) {
        return "UNSIGNED";
    } else if (token_type == TokenType::L_BRACE) {
        return "L_BRACE";
    } else if (token_type == TokenType::R_BRACE) {
        return "R_BRACE";
    } else if (token_type == TokenType::SEMICOLON) {
        return "SEMICOLON";
    } else if (token_type == TokenType::L_PARENTHESES) {
        return "L_PARENTHESES";
    } else if (token_type == TokenType::R_PARENTHESES) {
        return "R_PARENTHESES";
    } else if (token_type == TokenType::MULTIPLY) {
        return "MULTIPLY";
    } else if (token_type == TokenType::PLUS) {
        return "PLUS";
    } else if (token_type == TokenType::MINUS) {
        return "MINUS";
    } else if (token_type == TokenType::TILDE) {
        return "TILDE";
    } else if (token_type == TokenType::DIVISION) {
        return "DIVISION";
    } else if (token_type == TokenType::PERCENT) {
        return "PERCENT";
    } else if (token_type == TokenType::AND) {
        return "AND";
    } else if (token_type == TokenType::CARET) {
        return "CARET";
    } else if (token_type == TokenType::OR) {
        return "OR";
    } else if (token_type == TokenType::L_BRACKET) {
        return "L_BRACKET";
    } else if (token_type == TokenType::R_BRACKET) {
        return "R_BRACKET";
    } else if (token_type == TokenType::COMMA) {
        return "COMMA";
    } else if (token_type == TokenType::L_GUILLEMET) {
        return "L_GUILLEMET";
    } else if (token_type == TokenType::R_GUILLEMET) {
        return "R_GUILLEMET";
    } else if (token_type == TokenType::INTEGER) {
        return "INTEGER";
    } else if (token_type == TokenType::ID) {
        return "ID";
    } else if (token_type == TokenType::STRING) {
        return "STRING";
    }

    return "ERROR";
}

bool Keyword::IsQuotation(const char &input_char) {
    return input_char == '"';
}

bool Keyword::IsL(const char &input_char) {
    return input_char == 'L' || input_char == 'l';
}

bool Keyword::IsLowerSignal(const char &input_char) {
    return input_char == '<';
}

bool Keyword::IsUpperSignal(const char &input_char) {
    return input_char == '>';
}

bool Keyword::IsExcapeSequence(const char &input_char) {
    return input_char == 'b' || input_char == 't' ||
            input_char == 'n' || input_char == 'f' ||
            input_char == 'r' || input_char == '\\' ||
            input_char == '\"';
}

bool Keyword::IsSlant(const char &input_char) {
    return input_char == '\\';
}

bool Keyword::IsEndOfFile(const char &input_char) {
    return input_char == '\u0000';
}

bool Keyword::IsBoolean(const std::string &input_string, std::string &token) {
    if  (input_string == "TRUE" || input_string == "FALSE") {
        token = "BOOLEAN";
        return true;
    } else {
        return false;
    }
}
