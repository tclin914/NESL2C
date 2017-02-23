#ifndef __NODE_H__
#define __NODE_H__

struct nodeType;
#include "type.h"
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

    /*param & typeDef*/
    int paramcount;
    int declarecount;

    /* Tuple */
    int tuplecount;
    int childcount;
    struct nodeType * tuplenode;

    /* Values for general use */
    int iValue;
    double rValue;
    char valueValid;
    char *string;
    struct nodeType * typeNode;
    
    /* Indicates which OP */
    char op;
    
    
    enum StdType valueType;
    
    struct SymTableEntry *entry;
    struct SymTable *table; 
};

struct nodeType* newNode(int type);
void deleteNode(struct nodeType* node);
void addChild(struct nodeType *node, struct nodeType *child);
void printTree(struct nodeType *node, int ident);

#endif

