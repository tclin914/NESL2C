#ifndef __PFCODEGEN_H___
#define __PFCODEGEN_H___

#define MAX 10

void pfcodegen(FILE *fptr, struct nodeType* node);
int inserttmp(struct nodeType* node);
int insertelm(struct nodeType* node);
void pfcheck(struct nodeType* node);

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
#endif
