#ifndef __VISITOR_H__
#define __VISITOR_H__

struct treenode;

typedef struct visitor_type {
  
  void (*visit)(struct treenode*);

} visitor;

#endif
