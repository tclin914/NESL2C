#ifndef __CODEGENCHECK_H___
#define __CODEGENCHECK_H___

int inserttmp(struct nodeType* node);
int insertelm(struct nodeType* node);
void pfcheck(struct nodeType* node);
//void deleteREF(char *s, int num);
void deleteREF(int start, int end);
void DECREF(FILE* fptr, int n);
void insertREF(char *s, enum StdType type, struct nodeType *link);
void phase1(FILE *fptr, struct nodeType* node);
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
int globalrefcnt;
#define MAX 10
#endif
