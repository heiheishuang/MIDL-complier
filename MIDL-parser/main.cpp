#include <iostream>
#include "Parser.h"

int main(int argc, char **argv) {
    Parser parser;

    parser.Run("../test/base_type_spec/test9");
    parser.Run("../test/base_type_spec/test10_with_error");
    parser.Run("../test/declarators/test11");
    parser.Run("../test/declarators/test12_with_error");
    parser.Run("../test/expr/test13");
    parser.Run("../test/expr/test14_with_error");
    parser.Run("../test/member_list/test3");
    parser.Run("../test/member_list/test4_with_error");
    parser.Run("../test/member_list/test5_with_error");

    parser.Run("../test/struct_type/test0");
    parser.Run("../test/struct_type/test1_with_error");
    parser.Run("../test/struct_type/test2_with_error");

    parser.Run("../test/type_spec/test6");
    parser.Run("../test/type_spec/test7_with_error");
    parser.Run("../test/type_spec/test8_with_error");

    parser.Run("../test/test15_complex");

    parser.Run("../test/zys_test/test1");
    parser.Run("../test/zys_test/test2");
    parser.Run("../test/zys_test/test3");
    parser.Run("../test/zys_test/test4");

    return 0;
}
