//
// Created by Zou on 4/18/21.
//

#include <string>
#include "ErrorMsgBuilder.h"


std::string
ErrorMsgBuilder::BuildErrorMsg(const std::string &file_name,
                               const std::string &error,
                               int line_number,
                               int col_number,
                               char error_character) {
    return file_name + ":" +
           std::to_string(line_number) + ":" +
           std::to_string(col_number) + " error: \"" +
           error_character + "\", " + error + "\r\n";
}

std::string
ErrorMsgBuilder::BuildErrorMsg(const std::string &file_name,
                               const std::string &error,
                               int line_number,
                               int col_number,
                               const std::string &error_character) {
    return file_name + ":" +
           std::to_string(line_number) + ":" +
           std::to_string(col_number) + " error: \"" +
           error_character + "\", " + error + "\r\n";
}

std::string ErrorMsgBuilder::BuildErrorMsg(const std::string &error) {
    return error + "\r\n";
}
