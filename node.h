#ifndef __NODE_H__
#define __NODE_H__

struct nodeType;

#define VALUE_INVALID   0
#define VALUE_I_VALID   1
#define VALUE_R_VALID   2

#define NODE_TOKEN          1
#define NODE_OP             2

// typeToken
#define NODE_INT            3
#define NODE_FLOAT          4
#define NODE_BOOL           5
#define NODE_CHAR           6

// NODE_TYPE 
#define NODE_TYPE_INT       7
#define NODE_TYPE_FLOAT     8
#define NODE_TYPE_CHAR      9
#define NODE_TYPE_BOOL      10

// 
#define NODE_ASSIGN_STMT    11
#define NODE_SYM_REF        12
#define NODE_TYPE_ARRAY     13
#define NODE_ARR_REF        14

//lazy man
#define NODE_EXP            15
#define NODE_SEQ            16
#define NODE_IFSTMT         17
#define NODE_THENSTMT       18
#define NODE_ELSESTMT       19

#define NODE_NESL           20

#define NODE_LIST           50
#define NODE_IFELSE         52

#define NODE_FUNC           94
#define NODE_PROC           95
#define NODE_STMT           96
#define NODE_CMP_STMT       97
#define NODE_PROC_AND_FUNC_DECL 98
#define NODE_VAR_OR_PROC    99
#define NODE_ERROR          100
#define NODE_EMPTY          101

#define OP_ADD  1
#define OP_SUB  2
#define OP_MUL  3
#define OP_DIV  4
#define OP_GT   5
#define OP_LT   6
#define OP_EQ   7
#define OP_GE   8
#define OP_LE   9
#define OP_NE   10
#define OP_NOT  11

#include "symtab.h"
struct nodeType {
    int nodeType;
    struct nodeType *parent;
    struct nodeType *child;
    struct nodeType *lsibling;
    struct nodeType *rsibling;

    /* Attribute for NODE_TOKEN */
    int tokenType;

    /* items for Array */
    int counts;
    int idxstart;
    int idxend;
    int arraydepth;
    struct nodeType *arraynext;

    /* Values for general use */
    int iValue;
    double rValue;
    char valueValid;
    char *string;
    
    /* Indicates which OP */
    char op;

    enum StdType valueType;
    struct SymTableEntry *entry;
};

struct nodeType* newNode(int type);
void deleteNode(struct nodeType* node);
void addChild(struct nodeType *node, struct nodeType *child);
void printTree(struct nodeType *node, int ident);

#endif

