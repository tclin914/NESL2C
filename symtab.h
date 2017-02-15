#ifndef __SYMTAB_H___
#define __SYMTAB_H___

#include <stdio.h>
#include "node.h"
void semanticCheck(struct nodeType* node);


// TODO 
// 1. add TypeArray when detect '[]' stuff 
// 2. use the most outside type to represent the node.
// 3. add TypeChar for char or string.
enum StdType {TypeInt=1, TypeFloat, TypeChar, TypeBool, 
              TypeSEQ_I,TypeSEQ_F,TypeSEQ_C,TypeSEQ_B,TypeSEQ,
              TypeTuple1,TypeTuple2,TypeTuple3,TypeTuple4,TypeTuple};

// TODO 
// 1. bind the parameter name with the typedefine nodes.
// 2. add the parameter names into symtable during analysis.
struct SymTableEntry {
    char name[100];
    enum StdType type;
    struct nodeType *link;
};

struct SymTable {
    struct SymTable * parent ;
    int size;
    struct SymTableEntry entries[100];
};

//extern struct SymTable SymbolTable;

#endif

