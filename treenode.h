#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

#include "treenodetype.h"

struct visitor;

typedef struct treenode {
    // type 
    NodeType node_type; 
    ValueType value_type;

    // value
    int int_val;
    float float_val;
    char *str_val;

    // child
    struct treenode *left;
    struct treenode *right;

    // sibling
    struct treenode *sibling;

} treenode;

treenode *createNode(NodeType node_type);

#endif
