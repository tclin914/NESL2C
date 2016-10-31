#ifndef __SYMTAB_H___
#define __SYMTAB_H___

#include "node.h"
void semanticCheck(struct nodeType* node);

enum StdType {TypeInt, TypeReal, TypeArray};

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

