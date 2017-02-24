#ifndef __PFCODEGEN_H___
#define __PFCODEGEN_H___

void pfcodegen(FILE *fptr, struct nodeType* node);
int inserttmp(struct nodeType* node);
int insertelm(struct nodeType* node);
void pfcheck(struct nodeType* node);

#endif
