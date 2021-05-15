//
// Created by Zou on 4/18/21.
//

#ifndef MIDL_SCANNER_SCANNER_H
#define MIDL_SCANNER_SCANNER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "utils/Keyword.h"
#include "utils/ErrorMsgBuilder.h"
#include "DFA.h"
#include "utils/TokenType.h"

class Scanner {
public:
    Scanner();
    virtual ~Scanner();
    std::vector<TokenType> Run(const std::string &input_file_name,
                               const std::string &output_token_file_name);

private:
    void ScanFile(std::vector<TokenType> &token_type_list);
    void ScanLine(const std::string &each_line,
                  int line_number,
                  std::vector<TokenType> &token_type_list);

    std::ifstream input_file_;
    std::string input_file_name_;
    std::ofstream output_token_file_;
    std::ofstream output_error_file_;

    DFA current_dfa_state_;
    std::shared_ptr<Keyword> p_keyword;
    std::string current_word_;

    INTEGER  current_integer_state_{INTEGER::INTEGER_0};
    STRING current_string_state_{STRING::STRING_0};
    ID current_id_state_{ID::ID_0};
    DOUBLE_SIGNAL current_double_state_{DOUBLE_SIGNAL::DOUBLE_SIGNAL_0};
};


#endif //MIDL_SCANNER_SCANNER_H
