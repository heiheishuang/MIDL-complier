//
// Created by Zou on 4/18/21.
//

#ifndef MIDL_SCANNER_ERRORMSGBUILDER_H
#define MIDL_SCANNER_ERRORMSGBUILDER_H

#include <string>

class ErrorMsgBuilder {
public:
    static std::string BuildErrorMsg(const std::string &file_name,
                                     const std::string &error,
                                     int line_number,
                                     int col_number,
                                     char error_character);

    static std::string BuildErrorMsg(const std::string &file_name,
                                     const std::string &error,
                                     int line_number,
                                     int col_number,
                                     const std::string &error_character);

    static std::string BuildErrorMsg(const std::string &error);
};


#endif //MIDL_SCANNER_ERRORMSGBUILDER_H
