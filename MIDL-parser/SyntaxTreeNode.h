//
// Created by Zou on 2021/5/10.
//

#ifndef MIDL_COMPLIER_SYNTAXTREENODE_H
#define MIDL_COMPLIER_SYNTAXTREENODE_H

#include <string>
#include <vector>
#include "utils/SyntaxType.h"

class SyntaxTreeNode {
public:
    SyntaxTreeNode();

    virtual ~SyntaxTreeNode();

    bool IsNull() const;

    void SetNodeType(NodeType node_type);

    const std::vector<SyntaxTreeNode> &GetChildrenList() const;

    void AddChildNode(const SyntaxTreeNode &syntax_tree_node);

    std::string ToString();

    void SetStmtType(STMT stmt_type);

    void SetExpType(EXP exp_type);

    void SetExpVal(ExpVal exp_val);

private:
    bool is_null_;
    NodeType node_type_;
    ExpVal exp_val_;
    STMT stmt_type_;
    EXP exp_type_;

    std::vector<SyntaxTreeNode> children_list_;

};


#endif //MIDL_COMPLIER_SYNTAXTREENODE_H
