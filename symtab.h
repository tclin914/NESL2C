#ifndef __SYMTAB_H___
#define __SYMTAB_H___

#include <stdio.h>
#include "node.h"
#include "type.h"
void semanticCheck(struct nodeType* node);

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
struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s) ;

#endif

