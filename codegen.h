#ifndef __CODEGEN_H___
#define __CODEGEN_H___

void printNESL(struct nodeType *node, FILE* yyout);
void printparam(FILE *fptr, struct nodeType* node);
void dumpTable(FILE *fptr, struct nodeType* node);
void APP3printFor(FILE *fptr, struct nodeType* node1, struct nodeType* node2);
void printNewSeq(FILE *fptr, struct nodeType* node);
void APP2printFor(FILE *fptr, struct nodeType* node1, struct nodeType* node2);
void codegen(FILE *fptr, struct nodeType* node);


#endif
