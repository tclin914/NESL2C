
#include <stdlib.h>

#include "treenode.h"

treenode *createNode(NodeType node_type) {
    treenode *node = (treenode*)malloc(sizeof(treenode));
    node->left = NULL;
    node->right = NULL;
    return node;
}
