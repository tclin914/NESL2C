#ifndef __CODEGEN_H___
#define __CODEGEN_H___

void printNESL(struct nodeType *node, FILE* yyout);
void printparam(FILE *fptr, struct nodeType* node);
void printtype(FILE *fptr, struct nodeType* type);
void dumpTable(FILE *fptr, struct nodeType* node);
void codegen(FILE *fptr, struct nodeType* node);
void printBindTuple(FILE *fptr, struct nodeType *node1, struct nodeType *node2);
void printEXPBINDTUPLE(FILE *fptr, struct nodeType* node1, struct nodeType *node2);
void gentuple(FILE* fptr);
void gentypes(FILE *fptr, struct nodeType *type);
int containArray(struct nodeType *node);
void printDECREF(FILE *fptr, struct nodeType *node);


#endif
