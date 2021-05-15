//
// Created by Zou on 2021/5/10.
//

#include "Parser.h"

Parser::Parser()
        : current_token_pose_(0) {
}

Parser::~Parser() = default;

void Parser::Run(const std::string &input_file_name) {
    current_token_pose_ = 0;

    if (output_syntax_tree_file.is_open()) {
        output_syntax_tree_file.close();
    }
    if (output_error_file_.is_open()) {
        output_error_file_.close();
    }

    p_scanner_ = std::make_shared<Scanner>();
    token_type_list_ = p_scanner_->Run(input_file_name, input_file_name);

    try {
        output_syntax_tree_file.open(input_file_name + "_syntax_tree.xml");
        output_error_file_.open(input_file_name + "_parser_error.txt");

    } catch (...) {
        std::cout << "Open file failed!" << std::endl;
        return;
    }

    SyntaxTreeNode syntax_tree_root_node = StartParse();
    PreOrder(syntax_tree_root_node);

    output_syntax_tree_file.close();
    output_error_file_.close();
}

void Parser::PreOrder(SyntaxTreeNode node) {
    if (!node.IsNull()) {
        output_syntax_tree_file << "<" + node.ToString() + ">\r\n\t";
        for (const auto &child : node.GetChildrenList()) {
            PreOrder(child);
        }
        output_syntax_tree_file << "</" + node.ToString() + ">\r\n\t";
    }
}

bool Parser::match(TokenType expected) {
    if (current_token_pose_ >= token_type_list_.size()) {
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg(
                "unexpected token: Error, No Token here!");
        return false;
    }
    if (token_type_list_.at(current_token_pose_) == expected) {
        current_token_pose_++;
        return true;
    } else {
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("unexpected token: " +
                                                             CurrentTokenToString());
        current_token_pose_++;
        return false;
    }
}

SyntaxTreeNode Parser::StartParse() {
    return StructType();
}

SyntaxTreeNode Parser::StructType() {
    SyntaxTreeNode struct_type_node;
    struct_type_node.SetNodeType(NodeType::STMT);
    struct_type_node.SetStmtType(STMT::STRUCT_TYPE);

    match(TokenType::STRUCT);

    match(TokenType::ID);

    match(TokenType::L_BRACE);

    SyntaxTreeNode member_list = MemberList();
    struct_type_node.AddChildNode(member_list);

    match(TokenType::R_BRACE);

    if (token_type_list_.at(current_token_pose_) == TokenType::EOF_TOKEN) {
        match(TokenType::EOF_TOKEN);
    }

    return struct_type_node;
}

SyntaxTreeNode Parser::MemberList() {
    SyntaxTreeNode member_list;
    member_list.SetNodeType(NodeType::STMT);
    member_list.SetStmtType(STMT::MEMBER_LIST);


    if (IsTypeSpec(
            token_type_list_.at(current_token_pose_),
            token_type_list_.at(current_token_pose_ + 1))) {

        SyntaxTreeNode type_spec = TypeSpec();
        SyntaxTreeNode declarators = Declarators();
        type_spec.AddChildNode(declarators);
        member_list.AddChildNode(type_spec);

        match(TokenType::SEMICOLON);

        while (IsTypeSpec(
                token_type_list_.at(current_token_pose_),
                token_type_list_.at(current_token_pose_ + 1))) {

            SyntaxTreeNode node_type_spec = TypeSpec();
            SyntaxTreeNode node_declarators = Declarators();
            node_type_spec.AddChildNode(node_declarators);
            member_list.AddChildNode(node_type_spec);

            match(TokenType::SEMICOLON);
        }
    }

    return member_list;
}

SyntaxTreeNode Parser::TypeSpec() {
    SyntaxTreeNode type_spec;
    type_spec.SetNodeType(NodeType::STMT);
    type_spec.SetStmtType(STMT::TYPE_SPEC);

    if (IsBaseTypeSpec(token_type_list_.at(current_token_pose_),
                       token_type_list_.at(current_token_pose_ + 1))) {
        SyntaxTreeNode base_type_spec = BaseTypeSpec();
        type_spec.AddChildNode(base_type_spec);
    } else if (IsStructType(token_type_list_.at(current_token_pose_))) {
        SyntaxTreeNode struct_type = StructType();
        type_spec.AddChildNode(struct_type);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("type spec error before " +
                                                             CurrentTokenToString());
    }

    return type_spec;
}

SyntaxTreeNode Parser::BaseTypeSpec() {
    SyntaxTreeNode base_type_spec;

    if (token_type_list_.at(current_token_pose_) == TokenType::BOOLEAN) {
        base_type_spec.SetNodeType(NodeType::EXP);
        base_type_spec.SetExpType(EXP::BASE_TYPE_SPEC);
        base_type_spec.SetExpVal(ExpVal::BOOLEAN);
        match(TokenType::BOOLEAN);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::CHAR) {
        base_type_spec.SetNodeType(NodeType::EXP);
        base_type_spec.SetExpType(EXP::BASE_TYPE_SPEC);
        base_type_spec.SetExpVal(ExpVal::CHAR);
        match(TokenType::CHAR);
    } else if (IsFloatingPtType(token_type_list_.at(current_token_pose_),
                                token_type_list_.at(current_token_pose_ + 1))) {
        base_type_spec.SetNodeType(NodeType::STMT);
        base_type_spec.SetStmtType(STMT::BASE_TYPE_SPEC);

        SyntaxTreeNode floating_pt_type = FloatingPtType();
        base_type_spec.AddChildNode(floating_pt_type);
    } else if (IsIntegerType(token_type_list_.at(current_token_pose_))) {
        base_type_spec.SetNodeType(NodeType::STMT);
        base_type_spec.SetStmtType(STMT::BASE_TYPE_SPEC);

        SyntaxTreeNode integer_type = IntegerType();
        base_type_spec.AddChildNode(integer_type);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("base type spec error before " +
                                                             CurrentTokenToString());
    }

    return base_type_spec;
}

SyntaxTreeNode Parser::FloatingPtType() {
    SyntaxTreeNode floating_pt_type;
    floating_pt_type.SetNodeType(NodeType::EXP);
    floating_pt_type.SetExpType(EXP::FLOATING_PT_TYPE);

    if (token_type_list_.at(current_token_pose_) == TokenType::FLOAT) {
        floating_pt_type.SetExpVal(ExpVal::FLOAT);
        match(TokenType::FLOAT);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::DOUBLE) {
        floating_pt_type.SetExpVal(ExpVal::DOUBLE);
        match(TokenType::DOUBLE);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::LONG) {
        floating_pt_type.SetExpVal(ExpVal::LONG_DOUBLE);
        match(TokenType::LONG);
        match(TokenType::DOUBLE);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("floating pt type error before " +
                                                             CurrentTokenToString());
    }
    return floating_pt_type;

}

SyntaxTreeNode Parser::IntegerType() {
    SyntaxTreeNode integer_type;
    integer_type.SetNodeType(NodeType::STMT);
    integer_type.SetStmtType(STMT::INTEGEER_TYPE);

    if (IsSignedInt(token_type_list_.at(current_token_pose_))) {
        SyntaxTreeNode signed_int = SignedInt();
        integer_type.AddChildNode(signed_int);
    } else if (IsUnsignedInt(token_type_list_.at(current_token_pose_))) {
        SyntaxTreeNode unsigned_int = UnsignedInt();
        integer_type.AddChildNode(unsigned_int);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("integer type error before " +
                                                             CurrentTokenToString());
    }

    return integer_type;

}

SyntaxTreeNode Parser::SignedInt() {
    SyntaxTreeNode signed_int;
    signed_int.SetNodeType(NodeType::EXP);
    signed_int.SetExpType(EXP::SIGNED_INT);

    if (token_type_list_.at(current_token_pose_) == TokenType::SHORT) {
        signed_int.SetExpVal(ExpVal::SHORT);
        match(TokenType::SHORT);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::INT8) {
        signed_int.SetExpVal(ExpVal::INT8);
        match(TokenType::INT8);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::INT16) {
        signed_int.SetExpVal(ExpVal::INT16);
        match(TokenType::INT16);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::INT32) {
        signed_int.SetExpVal(ExpVal::INT32);
        match(TokenType::INT32);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::INT64) {
        signed_int.SetExpVal(ExpVal::INT64);
        match(TokenType::INT64);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::LONG and
               token_type_list_.at(current_token_pose_ + 1) == TokenType::LONG) {
        signed_int.SetExpVal(ExpVal::LONG_LONG);
        match(TokenType::LONG);
        match(TokenType::LONG);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::LONG) {
        signed_int.SetExpVal(ExpVal::LONG);
        match(TokenType::LONG);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("signed int error before " +
                                                             CurrentTokenToString());
    }

    return signed_int;
}

SyntaxTreeNode Parser::UnsignedInt() {
    SyntaxTreeNode unsigned_int;
    unsigned_int.SetNodeType(NodeType::EXP);
    unsigned_int.SetExpType(EXP::UNSIGNED_INT);

    if (token_type_list_.at(current_token_pose_) == TokenType::UINT8) {
        unsigned_int.SetExpVal(ExpVal::UINT8);
        match(TokenType::UINT8);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UINT16) {
        unsigned_int.SetExpVal(ExpVal::UINT16);
        match(TokenType::UINT16);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UINT32) {
        unsigned_int.SetExpVal(ExpVal::UINT32);
        match(TokenType::UINT32);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UINT64) {
        unsigned_int.SetExpVal(ExpVal::UINT64);
        match(TokenType::UINT64);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UNSIGNED and
               token_type_list_.at(current_token_pose_ + 1) == TokenType::SHORT) {
        unsigned_int.SetExpVal(ExpVal::UNSIGNED_SHORT);
        match(TokenType::UNSIGNED);
        match(TokenType::SHORT);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UNSIGNED and
               token_type_list_.at(current_token_pose_ + 1) == TokenType::LONG and
               token_type_list_.at(current_token_pose_ + 2) == TokenType::LONG) {
        unsigned_int.SetExpVal(ExpVal::UNSIGNED_LONG_LONG);
        match(TokenType::UNSIGNED);
        match(TokenType::LONG);
        match(TokenType::LONG);
    } else if (token_type_list_.at(current_token_pose_) == TokenType::UNSIGNED and
               token_type_list_.at(current_token_pose_ + 1) == TokenType::LONG) {
        unsigned_int.SetExpVal(ExpVal::UNSIGNED_LONG);
        match(TokenType::UNSIGNED);
        match(TokenType::LONG);
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("unsigned int error before " +
                                                             CurrentTokenToString());
    }

    return unsigned_int;
}

SyntaxTreeNode Parser::Declarators() {
    SyntaxTreeNode declarators;
    declarators.SetNodeType(NodeType::STMT);
    declarators.SetStmtType(STMT::DECLARATORS);

    SyntaxTreeNode declarator = Declarator();
    declarators.AddChildNode(declarator);

    while (token_type_list_.at(current_token_pose_) == TokenType::COMMA) {
        match(TokenType::COMMA);
        SyntaxTreeNode node = Declarator();
        declarator.AddChildNode(node);
        declarator = node;
    }

    return declarators;
}

SyntaxTreeNode Parser::Declarator() {
    SyntaxTreeNode declarator;
    declarator.SetNodeType(NodeType::STMT);
    declarator.SetStmtType(STMT::DECLARATOR);

    match(TokenType::ID);

    if (token_type_list_.at(current_token_pose_) == TokenType::L_BRACKET) {
        SyntaxTreeNode exp_list = ExpList();
        declarator.AddChildNode(exp_list);
    }

    return declarator;
}

SyntaxTreeNode Parser::ExpList() {
    SyntaxTreeNode exp_list;
    exp_list.SetNodeType(NodeType::STMT);
    exp_list.SetStmtType(STMT::EXP_LIST);

    match(TokenType::L_BRACKET);

    SyntaxTreeNode or_expr = OrExpr();
    exp_list.AddChildNode(or_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::COMMA) {
        match(TokenType::COMMA);

        SyntaxTreeNode node = OrExpr();
        or_expr.AddChildNode(node);
        or_expr = node;
    }
    match(TokenType::R_BRACKET);

    return exp_list;
}

SyntaxTreeNode Parser::OrExpr() {
    SyntaxTreeNode or_expr;
    or_expr.SetNodeType(NodeType::STMT);
    or_expr.SetStmtType(STMT::OR_EXPR);

    SyntaxTreeNode xor_expr = XorExpr();
    or_expr.AddChildNode(xor_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::OR) {
        match(TokenType::OR);

        SyntaxTreeNode node = XorExpr();
        xor_expr.AddChildNode(node);
        xor_expr = node;
    }

    return or_expr;
}

SyntaxTreeNode Parser::XorExpr() {
    SyntaxTreeNode xor_expr;
    xor_expr.SetNodeType(NodeType::STMT);
    xor_expr.SetStmtType(STMT::XOR_EXPR);

    SyntaxTreeNode and_expr = AndExpr();
    xor_expr.AddChildNode(and_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::CARET) {
        match(TokenType::CARET);

        SyntaxTreeNode node = AndExpr();
        and_expr.AddChildNode(node);
        and_expr = node;
    }

    return xor_expr;
}

SyntaxTreeNode Parser::AndExpr() {
    SyntaxTreeNode and_expr;
    and_expr.SetNodeType(NodeType::STMT);
    and_expr.SetStmtType(STMT::AND_EXPR);

    SyntaxTreeNode shift_expr = ShiftExpr();
    and_expr.AddChildNode(shift_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::AND) {
        match(TokenType::AND);

        SyntaxTreeNode node = ShiftExpr();
        shift_expr.AddChildNode(node);
        shift_expr = node;
    }

    return and_expr;
}

SyntaxTreeNode Parser::ShiftExpr() {
    SyntaxTreeNode shift_expr;
    shift_expr.SetNodeType(NodeType::STMT);
    shift_expr.SetStmtType(STMT::SHIFT_EXPR);

    SyntaxTreeNode add_expr = AddExpr();
    shift_expr.AddChildNode(add_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::L_GUILLEMET or
           token_type_list_.at(current_token_pose_) == TokenType::R_GUILLEMET) {

        if (token_type_list_.at(current_token_pose_) == TokenType::L_GUILLEMET) {
            match(TokenType::L_GUILLEMET);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::R_GUILLEMET) {
            match(TokenType::R_GUILLEMET);
        } else {
            current_token_pose_++;
            output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("shift expr error before " +
                                                                 CurrentTokenToString());
        }

        SyntaxTreeNode node = AddExpr();
        add_expr.AddChildNode(node);
        add_expr = node;
    }

    return shift_expr;
}

SyntaxTreeNode Parser::AddExpr() {
    SyntaxTreeNode add_expr;
    add_expr.SetNodeType(NodeType::STMT);
    add_expr.SetStmtType(STMT::ADD_EXPR);

    SyntaxTreeNode mult_expr = MultExpr();
    add_expr.AddChildNode(mult_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::PLUS or
           token_type_list_.at(current_token_pose_) == TokenType::MINUS) {

        if (token_type_list_.at(current_token_pose_) == TokenType::PLUS) {
            match(TokenType::PLUS);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::MINUS) {
            match(TokenType::MINUS);
        } else {
            current_token_pose_++;
            output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("add expr error before " +
                                                                 CurrentTokenToString());
        }

        SyntaxTreeNode node = MultExpr();
        mult_expr.AddChildNode(node);
        mult_expr = node;
    }

    return add_expr;
}

SyntaxTreeNode Parser::MultExpr() {
    SyntaxTreeNode mult_expr;
    mult_expr.SetNodeType(NodeType::STMT);
    mult_expr.SetStmtType(STMT::MULT_EXPR);

    SyntaxTreeNode unary_expr = UnaryExpr();
    mult_expr.AddChildNode(unary_expr);

    while (token_type_list_.at(current_token_pose_) == TokenType::MULTIPLY or
           token_type_list_.at(current_token_pose_) == TokenType::DIVISION or
           token_type_list_.at(current_token_pose_) == TokenType::PERCENT) {

        if (token_type_list_.at(current_token_pose_) == TokenType::MULTIPLY) {
            match(TokenType::MULTIPLY);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::DIVISION) {
            match(TokenType::DIVISION);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::PERCENT) {
            match(TokenType::PERCENT);
        } else {
            current_token_pose_++;
            output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("mult expr error before " +
                                                                 CurrentTokenToString());
        }

        SyntaxTreeNode node = MultExpr();
        mult_expr.AddChildNode(node);
        mult_expr = node;
    }

    return unary_expr;
}

SyntaxTreeNode Parser::UnaryExpr() {
    SyntaxTreeNode unary_expr;
    unary_expr.SetNodeType(NodeType::STMT);
    unary_expr.SetStmtType(STMT::UNARY_EXPR);

    if (token_type_list_.at(current_token_pose_) == TokenType::PLUS or
        token_type_list_.at(current_token_pose_) == TokenType::MINUS or
        token_type_list_.at(current_token_pose_) == TokenType::TILDE) {

        if (token_type_list_.at(current_token_pose_) == TokenType::PLUS) {
            match(TokenType::PLUS);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::MINUS) {
            match(TokenType::MINUS);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::TILDE) {
            match(TokenType::TILDE);
        } else {
            current_token_pose_++;
            output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("unary expr error before " +
                                                                 CurrentTokenToString());
        }
    }

    if (token_type_list_.at(current_token_pose_) == TokenType::INTEGER or
        token_type_list_.at(current_token_pose_) == TokenType::STRING or
        token_type_list_.at(current_token_pose_) == TokenType::BOOLEAN) {

        if (token_type_list_.at(current_token_pose_) == TokenType::INTEGER) {
            match(TokenType::INTEGER);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::STRING) {
            match(TokenType::STRING);
        } else if (token_type_list_.at(current_token_pose_) == TokenType::BOOLEAN) {
            match(TokenType::BOOLEAN);
        } else {
            current_token_pose_++;
            output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("unary expr error before " +
                                                                 CurrentTokenToString());
        }
    } else {
        current_token_pose_++;
        output_error_file_ << ErrorMsgBuilder::BuildErrorMsg("unary expr error before " +
                                                             CurrentTokenToString());
    }
    return unary_expr;
}


std::string Parser::CurrentTokenToString() {
    return Keyword::TokenTypeToString(token_type_list_.at(current_token_pose_));
}

bool Parser::IsSignedInt(TokenType tokenType) {
    if (tokenType == TokenType::SHORT or tokenType == TokenType::INT16 or
        tokenType == TokenType::INT32 or tokenType == TokenType::INT64 or
        tokenType == TokenType::INT8 or tokenType == TokenType::LONG) {
        return true;
    }
    return false;
}

bool Parser::IsUnsignedInt(TokenType tokenType) {
    if (tokenType == TokenType::UNSIGNED or tokenType == TokenType::UINT16 or
        tokenType == TokenType::UINT32 or tokenType == TokenType::UINT64 or
        tokenType == TokenType::UINT8) {
        return true;
    }

    return false;
}

bool Parser::IsFloatingPtType(TokenType tokenType, TokenType nextToken) {
    if (tokenType == TokenType::FLOAT or tokenType == TokenType::DOUBLE) {
        return true;
    }

    if (tokenType == TokenType::LONG and nextToken == TokenType::DOUBLE) {
        return true;
    }
    return false;
}

bool Parser::IsIntegerType(TokenType tokenType) {
    if (IsUnsignedInt(tokenType) or IsSignedInt(tokenType)) {
        return true;
    }
    return false;
}

bool Parser::IsBaseTypeSpec(TokenType tokenType, TokenType nextToken) {
    if (IsFloatingPtType(tokenType, nextToken)) {
        return true;
    }

    if (IsIntegerType(tokenType)) {
        return true;
    }

    if (tokenType == TokenType::CHAR or tokenType == TokenType::BOOLEAN) {
        return true;
    }

    return false;
}

bool Parser::IsStructType(TokenType tokenType) {
    if (tokenType == TokenType::STRUCT) {
        return true;
    }
    return false;
}

bool Parser::IsTypeSpec(TokenType tokenType, TokenType nextToken) {

    if (IsBaseTypeSpec(tokenType, nextToken) or IsStructType(tokenType)) {
        return true;
    }
    return false;

}

