//
// Created by Zou on 4/18/21.
//

#include "Scanner.h"

Scanner::Scanner() : current_dfa_state_(DFA::START){
    p_keyword = std::make_shared<Keyword>();
}

Scanner::~Scanner() = default;

std::vector<TokenType> Scanner::Run(const std::string &input_file_name, const std::string &output_token_file_name) {
    if (input_file_.is_open()) {
        input_file_.close();
    }
    if (output_token_file_.is_open()) {
        input_file_.close();
    }
    if (output_error_file_.is_open()) {
        input_file_.close();
    }

    std::vector<TokenType> token_type_list;

    try {
        input_file_.open(input_file_name + ".txt");
        input_file_name_ = input_file_name;

        output_token_file_.open(output_token_file_name + "_tokens.txt");
        output_error_file_.open(output_token_file_name + "_scanner_error.txt");
    } catch (...) {
        std::cout << "Open file failed!" << std::endl;
        return token_type_list;
    }

    ScanFile(token_type_list);

    input_file_.close();
    output_token_file_.close();
    output_error_file_.close();

    return token_type_list;
}

void Scanner::ScanFile(std::vector<TokenType> &token_type_list) {
    current_dfa_state_ = DFA::START;

    std::string each_line;
    int line_number = 1;
    while (!input_file_.eof()) {
        std::getline(input_file_, each_line);
        if (!each_line.empty()) {
            if (each_line.at(each_line.length() - 1) != '\r') {
                each_line.append("\r");
            }
            ScanLine(each_line, line_number, token_type_list);
        }
        ++line_number;
    }

    token_type_list.emplace_back(TokenType::EOF_TOKEN);
}

void Scanner::ScanLine(const std::string &each_line, int line_number, std::vector<TokenType> &token_type_list) {
    int char_index = 0;
    bool is_index_forward = false;
    bool last_char_is_blank = true;

    while (char_index < each_line.size()) {
        const char current_char = each_line.at(char_index);

        switch (current_dfa_state_) {

            case DFA::START: {
                if (p_keyword->IsBlankChar(current_char)) {
                    is_index_forward = true;
                    current_dfa_state_ = DFA::START;
                    output_token_file_ << current_char;
                    last_char_is_blank = true;

                } else if (p_keyword->IsEndOfFile(current_char)) {
                    is_index_forward = true;
                    current_dfa_state_ = DFA::START;
                    last_char_is_blank = true;
                } else if (p_keyword->IsLetter(current_char)) {
                    is_index_forward = false;
                    current_dfa_state_ = DFA::ID;
                    current_id_state_ = ID::ID_0;
                    current_word_.clear();

                } else if (p_keyword->IsDigit(current_char)) {
                    is_index_forward = false;
                    current_dfa_state_ = DFA::INTEGER;
                    current_integer_state_ = INTEGER::INTEGER_0;

                } else if (p_keyword->IsQuotation(current_char)) {
                    is_index_forward = false;
                    current_dfa_state_ = DFA::STRING;
                    current_string_state_ = STRING::STRING_0;
                } else if (p_keyword->IsDoubleCharOperatorFirst(current_char)) {
                    is_index_forward = false;
                    current_dfa_state_ = DFA::DOUBLE_SIGNAL;
                    current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_0;
                    current_word_.clear();
                } else {
                    std::string token;
                    if (p_keyword->IsSingleCharOperator(current_char, token)) {
                        is_index_forward = true;
                        current_dfa_state_ = DFA::START;
                        if (!last_char_is_blank) {
                            output_token_file_ << " ";
                        }
                        output_token_file_ << token;
                        token_type_list.emplace_back(Keyword::StringToTokenType(token));
                        last_char_is_blank = false;

                    } else {
                        is_index_forward = true;
                        current_dfa_state_ = DFA::START;
                        output_error_file_
                                << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                  "incorrect morphology!",
                                                                  line_number,
                                                                  char_index + 1,
                                                                  current_char);
                    }
                }

                break;
            }

            case DFA::STRING: {

                switch (current_string_state_) {
                    case STRING::STRING_0: {
                        if (p_keyword->IsQuotation(current_char)) {
                            is_index_forward = true;
                            current_string_state_ = STRING::STRING_1;
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_string_state_ = STRING::STRING_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case STRING::STRING_1: {
                        if (p_keyword->IsQuotation(current_char)) {
                            is_index_forward = true;
                            current_string_state_ = STRING::STRING_4;
                        } else if (p_keyword->IsStringAllow(current_char)) {
                            is_index_forward = true;
                            current_string_state_ = STRING::STRING_1;
                        } else if (p_keyword->IsSlant(current_char)) {
                            is_index_forward = true;
                            current_string_state_ = STRING::STRING_2;
                        } else if (p_keyword->IsEndOfFile(current_char)) {
                            is_index_forward = true;
                            current_dfa_state_ = DFA::START;
                            current_string_state_ = STRING::STRING_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_string_state_ = STRING::STRING_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case STRING::STRING_2: {
                        if (p_keyword->IsExcapeSequence(current_char)) {
                            is_index_forward = true;
                            current_string_state_ = STRING::STRING_1;
                        } else {
                            is_index_forward = true;
                            current_dfa_state_ = DFA::START;
                            current_string_state_ = STRING::STRING_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }

                        break;
                    }

                    case STRING::STRING_4: {
                        is_index_forward = false;
                        current_dfa_state_ = DFA::START;
                        current_string_state_ = STRING::STRING_4;
                        std::string token;

                        if (!last_char_is_blank) {
                            output_token_file_ << " ";
                        }
                        last_char_is_blank = false;
                        output_token_file_ << "STRING";
                        token_type_list.emplace_back(Keyword::StringToTokenType("STRING"));
                        break;
                    }
                }
                break;
            }

            case DFA::INTEGER: {

                switch (current_integer_state_) {
                    case INTEGER::INTEGER_0: {
                        if (p_keyword->IsZero(current_char)) {
                            is_index_forward = true;
                            current_integer_state_ = INTEGER::INTEGER_34;
                        } else {
                            is_index_forward = true;
                            current_integer_state_ = INTEGER::INTEGER_1234;
                        }
                        break;
                    }

                    case INTEGER::INTEGER_1234: {
                        if (p_keyword->IsL(current_char)) {
                            is_index_forward = true;
                            current_integer_state_ = INTEGER::INTEGER_4;
                        } else if (p_keyword->IsDigit(current_char)) {
                            is_index_forward = true;
                            current_integer_state_ = INTEGER::INTEGER_1234;
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_integer_state_ = INTEGER::INTEGER_0;

                            if (!last_char_is_blank) {
                                output_token_file_ << " ";
                            }
                            last_char_is_blank = false;

                            output_token_file_ << "INTEGER";
                            token_type_list.emplace_back(Keyword::StringToTokenType("INTEGER"));

                        }
                        break;
                    }

                    case INTEGER::INTEGER_34: {
                        if (p_keyword->IsL(current_char)) {
                            is_index_forward = true;
                            current_integer_state_ = INTEGER::INTEGER_4;
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_integer_state_ = INTEGER::INTEGER_0;

                            if (!last_char_is_blank) {
                                output_token_file_ << " ";
                            }
                            last_char_is_blank = false;

                            output_token_file_ << "INTEGER";
                            token_type_list.emplace_back(Keyword::StringToTokenType("INTEGER"));
                        }
                        break;
                    }

                    case INTEGER::INTEGER_4: {
                        is_index_forward = false;
                        current_dfa_state_ = DFA::START;
                        current_integer_state_ = INTEGER::INTEGER_0;

                        if (!last_char_is_blank) {
                            output_token_file_ << " ";
                        }
                        last_char_is_blank = false;
                        output_token_file_ << "INTEGER";
                        token_type_list.emplace_back(Keyword::StringToTokenType("INTEGER"));
                        break;
                    }
                }

                break;

            }

            case DFA::ID: {

                switch (current_id_state_) {
                    case ID::ID_0: {
                        if (p_keyword->IsLetter(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_1234;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_id_state_ = ID::ID_0;
                        }
                        break;
                    }

                    case ID::ID_1234: {
                        if (p_keyword->IsUnderLine(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_3;
                            current_word_.push_back(current_char);
                        } else if (p_keyword->IsLetterOrDigit(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_234;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_id_state_ = ID::ID_0;

                            std::string token;
                            if (p_keyword->IsKeyWords(current_word_, token)) {
                                if (!last_char_is_blank) {
                                    output_token_file_ << " ";
                                }
                                output_token_file_ << token;
                                token_type_list.emplace_back(Keyword::StringToTokenType(token));
                                last_char_is_blank = false;
                            } else {
                                if (!last_char_is_blank) {
                                    output_token_file_ << " ";
                                }
                                output_token_file_ << "ID";
                                token_type_list.emplace_back(Keyword::StringToTokenType("ID"));
                                last_char_is_blank = false;
                            }

                        }
                        break;
                    }

                    case ID::ID_3: {
                        if (p_keyword->IsLetterOrDigit(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_234;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_id_state_ = ID::ID_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case ID::ID_234: {
                        if (p_keyword->IsUnderLine(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_3;
                            current_word_.push_back(current_char);
                        } else if (p_keyword->IsLetterOrDigit(current_char)) {
                            is_index_forward = true;
                            current_id_state_ = ID::ID_234;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_id_state_ = ID::ID_0;

                            std::string token;
                            if (p_keyword->IsBoolean(current_word_, token)) {
                                if (!last_char_is_blank) {
                                    output_token_file_ << " ";
                                }
                                output_token_file_ << token;
                                token_type_list.emplace_back(Keyword::StringToTokenType(token));
                                last_char_is_blank = false;

                            } else if (p_keyword->IsKeyWords(current_word_, token)) {
                                if (!last_char_is_blank) {
                                    output_token_file_ << " ";
                                }
                                output_token_file_ << token;
                                token_type_list.emplace_back(Keyword::StringToTokenType(token));
                                last_char_is_blank = false;

                            } else {
                                if (!last_char_is_blank) {
                                    output_token_file_ << " ";
                                }
                                output_token_file_ << "ID";
                                token_type_list.emplace_back(Keyword::StringToTokenType("ID"));
                                last_char_is_blank = false;
                            }
                        }
                        break;
                    }
                }

                break;
            }

            case DFA::DOUBLE_SIGNAL: {
                switch (current_double_state_) {
                    case DOUBLE_SIGNAL::DOUBLE_SIGNAL_0: {
                        if (p_keyword->IsLowerSignal(current_char)) {
                            is_index_forward = true;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_1;
                            current_word_.push_back(current_char);
                        } else if (p_keyword->IsUpperSignal(current_char)) {
                            is_index_forward = true;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_2;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case DOUBLE_SIGNAL::DOUBLE_SIGNAL_1: {
                        if (p_keyword->IsLowerSignal(current_char)) {
                            is_index_forward = true;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_3;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case DOUBLE_SIGNAL::DOUBLE_SIGNAL_2: {
                        if (p_keyword->IsUpperSignal(current_char)) {
                            is_index_forward = true;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_3;
                            current_word_.push_back(current_char);
                        } else {
                            is_index_forward = false;
                            current_dfa_state_ = DFA::START;
                            current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_0;
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                        break;
                    }

                    case DOUBLE_SIGNAL::DOUBLE_SIGNAL_3: {
                        is_index_forward = false;
                        current_dfa_state_ = DFA::START;
                        current_double_state_ = DOUBLE_SIGNAL::DOUBLE_SIGNAL_0;
                        std::string token;

                        if (p_keyword->IsDoubleCharOperatorSecond(
                                current_word_.at(0),
                                current_word_.at(1),
                                token)) {

                            if (!last_char_is_blank) {
                                output_token_file_ << " ";
                            }
                            output_token_file_ << token;
                            token_type_list.emplace_back(Keyword::StringToTokenType(token));
                            last_char_is_blank = false;

                        } else {
                            output_error_file_
                                    << ErrorMsgBuilder::BuildErrorMsg(input_file_name_,
                                                                      "before, has incorrect definition morphology!",
                                                                      line_number,
                                                                      char_index + 1,
                                                                      current_char);
                        }
                    }

                }
            }

        }

        if (is_index_forward) {
            char_index++;
        }
    }
}
