#ifndef __CODEGENCHECK_H___
#define __CODEGENCHECK_H___

int inserttmp(struct nodeType* node);
int insertelm(struct nodeType* node);
void pfcheck(struct nodeType* node);
void DecRefCountForContainedArray(FILE* fptr, struct nodeType *child);
//void deleteREF(char *s, int num);
void deleteREF(int start, int end);
void DECREF(FILE* fptr, int n);
int findREF(char *s);
void insertREF(char *s, enum StdType type, struct nodeType *link);
void printGlobalVar(FILE *fptr, struct nodeType* node);
void printAddREF(FILE *fptr, char* string, enum StdType type, struct nodeType* node);

struct RefTableEntry {
    char name[100];
    enum StdType type;
    struct nodeType *link;
};

struct RefTable{
  int size;
  struct RefTableEntry entries[100];
};

extern struct RefTable refTable;
extern int issrand;
#define MAX 1000 
#endif
