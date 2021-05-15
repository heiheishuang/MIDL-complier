//
// Created by Zou on 2021/5/10.
//

#ifndef MIDL_COMPLIER_PARSER_H
#define MIDL_COMPLIER_PARSER_H

#include "Scanner.h"
#include "SyntaxTreeNode.h"

class Parser {
public:
    Parser();

    virtual ~Parser();

    void Run(const std::string &input_file_name);

    void PreOrder(SyntaxTreeNode node);

    bool match(TokenType expected);

    SyntaxTreeNode StartParse();

    SyntaxTreeNode StructType();

    SyntaxTreeNode MemberList();

    SyntaxTreeNode TypeSpec();

    SyntaxTreeNode BaseTypeSpec();

    SyntaxTreeNode FloatingPtType();

    SyntaxTreeNode IntegerType();

    SyntaxTreeNode SignedInt();

    SyntaxTreeNode UnsignedInt();

    SyntaxTreeNode Declarators();

    SyntaxTreeNode Declarator();

    SyntaxTreeNode ExpList();

    SyntaxTreeNode OrExpr();

    SyntaxTreeNode XorExpr();

    SyntaxTreeNode AndExpr();

    SyntaxTreeNode ShiftExpr();

    SyntaxTreeNode AddExpr();

    SyntaxTreeNode MultExpr();

    SyntaxTreeNode UnaryExpr();

    std::string CurrentTokenToString();

    static bool IsSignedInt(TokenType tokenType);

    static bool IsUnsignedInt(TokenType tokenType);

    static bool IsFloatingPtType(TokenType tokenType, TokenType nextToken);

    static bool IsIntegerType(TokenType tokenType);

    static bool IsBaseTypeSpec(TokenType tokenType, TokenType nextToken);

    static bool IsStructType(TokenType tokenType);

    static bool IsTypeSpec(TokenType tokenType, TokenType nextToken);

private:
    std::ofstream output_syntax_tree_file;
    std::ofstream output_error_file_;

    std::shared_ptr<Scanner> p_scanner_;
    std::vector<TokenType> token_type_list_;
    int current_token_pose_;
};


#endif //MIDL_COMPLIER_PARSER_H
