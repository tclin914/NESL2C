#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

#include "treenodetype.h"

struct visitor;

typedef struct node {
    // type 
    NodeType node_type; 
    ValueType value_type;

    // value
    int int_val;
    float float_val;
    char *str_val;

    // child
    struct node *left;
    struct node *right;

    // sibling
    struct node *sibling;

    void (*accept)(struct visitor*);

} treenode;

treenode *createNode(NodeType node_type);

#endif
