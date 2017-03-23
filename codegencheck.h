#ifndef __CODEGENCHECK_H___
#define __CODEGENCHECK_H___

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

#define MAX 10
#endif
