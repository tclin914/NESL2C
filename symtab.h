#ifndef __SYMTAB_H___
#define __SYMTAB_H___

#include <stdio.h>
#include "node.h"
void semanticCheck(struct nodeType* node);
void codegen(FILE* fptr,struct nodeType* node);


// TODO 
// 1. add TypeArray when detect '[]' stuff 
// 2. use the most outside type to represent the node.
// 3. add TypeChar for char or string.
enum StdType {TypeInt, TypeReal, TypeArray};

// TODO 
// 1. bind the parameter name with the typedefine nodes.
// 2. add the parameter names into symtable during analysis.
struct SymTableEntry {
    char name[100];
    enum StdType type;
    struct nodeType *link;
};

struct SymTable {
    int size;
    struct SymTableEntry entries[100];
};

extern struct SymTable SymbolTable;


#endif

