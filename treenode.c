#include <stdlib.h>

#include "treenode.h"

treenode *createNode(NodeType node_type) {

    treenode *node = (treenode*)malloc(sizeof(treenode));
    node->node_type = node_type;
    node->value_type = VALUE_NONE;
    node->left = NULL;
    node->right = NULL;

    return node;
}
