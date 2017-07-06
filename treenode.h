#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

#include "treenodetype.h"

typedef struct node {
    // type 
    NodeType node_type; 

    // value
    int int_val;
    float float_val;
    char *str_val;

    // child
    struct node *left;
    struct node *right;

    // sibling
    struct node *sibling;

} treenode;

treenode *createNode(NodeType node_type);

#endif
