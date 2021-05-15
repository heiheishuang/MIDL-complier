#include <iostream>

#include "Scanner.h"
int main(int argc, char** argv) {
    Scanner scanner;
    scanner.Run("../test/test_1_kw", "../test/result/test_1_kw");
    scanner.Run("../test/test_2_kw", "../test/result/test_2_kw");
    scanner.Run("../test/test_3_formula", "../test/result/test_3_formula");
    scanner.Run("../test/test_4_identifier", "../test/result/test_4_identifier");
    scanner.Run("../test/test_5_identifier_with_error", "../test/result/test_5_identifier_with_error");
    scanner.Run("../test/test_6_identifier_with_error", "../test/result/test_6_identifier_with_error");
    scanner.Run("../test/test_7_expression", "../test/result/test_7_expression");
    scanner.Run("../test/test_8_complex", "../test/result/test_8_complex");
    scanner.Run("../test/test_9_complex_with_escape", "../test/result/test_9_complex");
    scanner.Run("../test/zys_test_integer", "../test/result/zys_test_integer");
    scanner.Run("../test/zys_test_id", "../test/result/zys_test_id");
    scanner.Run("../test/zys_test_boolean", "../test/result/zys_test_boolean");
    scanner.Run("../test/zys_test_string", "../test/result/zys_test_string");
    scanner.Run("../test/zys_test_all", "../test/result/zys_test_all");

    return 0;
}