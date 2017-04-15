#ifndef __CODEGEN_H___
#define __CODEGEN_H___

void printNESL(struct nodeType *node, FILE* yyout);
void printparam(FILE *fptr, struct nodeType* node);
void printtype(FILE *fptr, enum StdType type);
void dumpTable(FILE *fptr, struct nodeType* node);
void codegen(FILE *fptr, struct nodeType* node);
void printBindTuple(FILE *fptr, struct nodeType *node1, struct nodeType *node2);
void printEXPBINDTUPLE(FILE *fptr, struct nodeType* node1, struct nodeType *node2);


#endif
