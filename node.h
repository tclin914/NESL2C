#ifndef __NODE_H__
#define __NODE_H__

struct nodeType;
#include "type.h"
#include "symtab.h"
struct nodeType {
    struct nodeType *parent;
    struct nodeType *child;
    struct nodeType *lsibling;
    struct nodeType *rsibling;
  
    /* node attribute */
    enum NodeNum nodeNum;
    struct DataType dataType;
    struct nodeType *typeNode;
    enum TokenNum token;
    enum OpType op;
  
    /* node value */
    int iValue;
    double rValue;
    char valueValid;
    char *string;
  
    /* items for Array */
    int counts;
    int idxstart;
    int idxend;
    int arraydepth;
  
    /*param & typeDef*/
    int paramcount;
    int declarecount;
  
    /* Tuple */
    int isvisited;
    int tuplecount;
    int childcount;
    struct nodeType * tuplenode;
  
    /* for codegen */
    struct DataType type;
    struct SymTableEntry *entry;
    struct SymTable *table; 
    int isEndofFunction;
    int needcounter;
    int isparallel_rr;
    int inserttmp;
    int infilter;
    int isParam;
  
    /*for variable declaration*/
    int mode;
};

struct nodeType* newNode(int type);
void deleteNode(struct nodeType* node);
void addChild(struct nodeType *node, struct nodeType *child);
void printTree(struct nodeType *node, int ident);

#endif

