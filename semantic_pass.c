
#include "semantic_pass.h"

#define SEMANTIC_PASS  1
#define SEMANTIC_ERROR 0

int semantic_pass(treenode *node) {

    int is_pass = SEMANTIC_PASS;
  
    if (node->left) {
        is_pass &= semantic_pass(node->left); 
    }

    if (node->right) {
        is_pass &= semantic_pass(node->right);
    }

    if (node->sibling) {
        is_pass &= semantic_pass(node->sibling);
    }

    switch (node->node_type) {
        case NODE_OR:
        case NODE_NOR:
        case NODE_XOR:
        case NODE_AND:
        case NODE_NAND:
            is_pass &= check_logic_op(node);
            break;

        case NODE_EQ:
        case NODE_NE:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
            break;

        case NODE_ADD:
        case NODE_SUB:
        case NODE_MUL:
        case NODE_DIV:
            is_pass &= check_arithmetic_op(node);
            break;

        case NODE_PP:
            is_pass &= check_pp(node);
            break;

        case NODE_CONST_INT:
            node->value_type = VALUE_INT;
            break;
        case NODE_CONST_FLOAT:
            node->value_type = VALUE_FLOAT; 
            break;
        case NODE_CONST_TRUE:
            node->value_type = VALUE_BOOL;
            break;
        case NODE_CONST_FALSE:
            node->value_type = VALUE_BOOL;
            break;
        case NODE_CONST_STRING:
            node->value_type = VALUE_STRING;
            break;
        default:
            is_pass &= SEMANTIC_PASS;
            break;
    }
    return is_pass;
}

static int check_logic_op(treenode *node) {
    
    if (node->left->value_type != VALUE_INT &&
            node->left->value_type != VALUE_BOOL) {
        return SEMANTIC_ERROR;        
    }

    // check childs is the same type
    if (node->left->value_type != node->right->value_type) {
        return SEMANTIC_ERROR;   
    }

    node->value_type = node->left->value_type;

    return SEMANTIC_PASS;
}

static int check_relational_op(treenode *node) {
    
    // TODO: char type
    if (node->left->value_type != VALUE_INT &&
            node->left->value_type != VALUE_FLOAT) {
        return SEMANTIC_ERROR;
    }
    
    if (node->left->value_type != node->right->value_type) {
        return SEMANTIC_ERROR;
    }

    node->value_type = VALUE_BOOL;

    return SEMANTIC_PASS;
}

static int check_arithmetic_op(treenode *node) {
        
    if (node->left->value_type != VALUE_INT &&
            node->left->value_type != VALUE_FLOAT) {
        return SEMANTIC_ERROR;
    }

    // check childs is the same type
    if (node->left->value_type != node->right->value_type) {
        return SEMANTIC_ERROR;
    }

    node->value_type = node->left->value_type;
    
    return SEMANTIC_PASS;
}

static int check_pp(treenode *node) {

    return SEMANTIC_PASS;
}
