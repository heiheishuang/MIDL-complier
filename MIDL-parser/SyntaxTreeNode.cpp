//
// Created by Zou on 2021/5/10.
//

#include "SyntaxTreeNode.h"

#include <string>
#include <vector>

SyntaxTreeNode::SyntaxTreeNode() :
        is_null_(false),
        exp_type_(EXP::UNDEFINED),
        exp_val_(ExpVal::UNDEFINED),
        node_type_(NodeType::UNDEFINED) {

}

SyntaxTreeNode::~SyntaxTreeNode() = default;

bool SyntaxTreeNode::IsNull() const {
    return is_null_;
}

void SyntaxTreeNode::SetNodeType(NodeType node_type) {
    node_type_ = node_type;
}

void SyntaxTreeNode::SetExpVal(ExpVal exp_val) {
    exp_val_ = exp_val;
}

void SyntaxTreeNode::SetStmtType(STMT stmt_type) {
    stmt_type_ = stmt_type;
}

void SyntaxTreeNode::SetExpType(EXP exp_type) {
    exp_type_ = exp_type;
}

const std::vector<SyntaxTreeNode> &SyntaxTreeNode::GetChildrenList() const {
    return children_list_;
}

void SyntaxTreeNode::AddChildNode(const SyntaxTreeNode &syntax_tree_node) {
    children_list_.emplace_back(syntax_tree_node);
}


std::string SyntaxTreeNode::ToString() {
    std::string output;
    switch (node_type_) {
        case NodeType::STMT: {
            switch (stmt_type_) {
                case STMT::STRUCT_TYPE: {
                    output += "STRUCT_TYPE";
                    break;
                }
                case STMT::MEMBER_LIST: {
                    output += "MEMBER_LIST";
                    break;
                }
                case STMT::TYPE_SPEC: {
                    output += "TYPE_SPEC";
                    break;
                }
                case STMT::DECLARATOR: {
                    output += "DECLARATOR";
                    break;
                }
                case STMT::DECLARATORS: {
                    output += "DECLARATORS";
                    break;
                }
                case STMT::BASE_TYPE_SPEC: {
                    output += "BASE_TYPE_SPEC";
                    break;
                }
                case STMT::INTEGEER_TYPE: {
                    output += "INTEGEER_TYPE";
                    break;
                }
                case STMT::EXP_LIST: {
                    output += "EXP_LIST";
                    break;
                }
                case STMT::OR_EXPR: {
                    output += "OR_EXPR";
                    break;
                }
                case STMT::XOR_EXPR: {
                    output += "XOR_EXPR";
                    break;
                }
                case STMT::AND_EXPR: {
                    output += "AND_EXPR";
                    break;
                }
                case STMT::ADD_EXPR: {
                    output += "ADD_EXPR";
                    break;
                }
                case STMT::SHIFT_EXPR: {
                    output += "SHIFT_EXPR";
                    break;
                }
                case STMT::MULT_EXPR: {
                    output += "MULT_EXPR";
                    break;
                }
                case STMT::UNARY_EXPR: {
                    output += "UNARY_EXPR";
                    break;
                }
                default: {
                    output += "UnknowSTMT";
                    break;
                }
            }
            break;
        }

        case NodeType::EXP: {
            switch (exp_type_) {
                case EXP::FLOATING_PT_TYPE: {
                    output += "FLOATING_PT_TYPE";
                    break;
                }
                case EXP::BASE_TYPE_SPEC: {
                    output += "BASE_TYPE_SPEC";
                    break;
                }
                case EXP::SIGNED_INT: {
                    output += "SIGNED_INT";
                    break;
                }
                case EXP::UNSIGNED_INT: {
                    output += "UNSIGNED_INT";
                    break;
                }
                default: {
                    output += "UnknownEXP";
                    break;
                }
            }
            break;
        }

        default:
            break;
    }

    return output;
}