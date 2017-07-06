#ifndef __TREE_NODE_TYPE_H__
#define __TREE_NODE_TYPE_H__

typedef enum {
    NODE_IFELSE,
    NODE_LET,

    NODE_ASSIGN,
    NODE_TUPLE,

    NODE_OR,
    NODE_NOR,
    NODE_XOR,

    NODE_AND,
    NODE_NAND,

    NODE_EQ,
    NODE_NE,
    NODE_LT,
    NODE_GT,
    NODE_LE,
    NODE_GE,

    NODE_ADD,
    NODE_SUB,
    NODE_PP,
    NODE_LARROW,

    NODE_MUL,
    NODE_DIV,
    NODE_RARROW,

    NODE_UPT,

    NODE_SHARP,
    NODE_AT,
    NODE_UMINUS,

    NODE_SEQ_REF,

    NODE_IN,

    NODE_ID,
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
    NODE_TYPE_BOOL,
    NODE_TYPE_CHAR,

    NODE_INT,
    NODE_FLOAT,
    NODE_TRUE,
    NODE_FALSE,
    NODE_STRING
} NodeType;

#endif
