#ifndef __CODEGEN_H___
#define __CODEGEN_H___

void printNESL(struct nodeType *node, FILE* yyout);
void printparam(FILE *fptr, struct nodeType* node);
void printtype(FILE *fptr, struct nodeType* node);
void dumpTable(FILE *fptr, struct nodeType* node);
void codegen(FILE *fptr, struct nodeType* node);
void printBindTuple(FILE *fptr, struct nodeType *node1, struct nodeType *node2);


#endif
