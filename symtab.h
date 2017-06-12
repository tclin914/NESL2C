#ifndef __SYMTAB_H___
#define __SYMTAB_H___

#include <stdio.h>
#include "node.h"
#include "type.h"

#define REFERENCE 0
#define FORCEDECLARE 1
#define NOROOT 2

void semanticCheck(struct nodeType* node);
struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link, int mode);
void typeAnalysis( struct nodeType *node);

// TODO 
// 1. bind the parameter name with the typedefine nodes.
// 2. add the parameter names into symtable during analysis.
struct SymTableEntry {
    char name[100];
    enum StdType type;
    struct nodeType *link;
    int isParam;
};

struct SymTable {
    struct SymTable * parent ;
    int size;
    struct SymTableEntry entries[100];
};


struct FuncTableEntry {
    char name[100];
    int renametimes;
};

struct FuncTable {
    int size;
    struct FuncTableEntry entries[100];
};
//extern struct SymTable SymbolTable;
struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s, int mode) ;

#endif

