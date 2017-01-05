#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}
struct SymTable SymbolTable;

struct SymTableEntry* findSymbol(char *s) {
    for(int i=0; i<SymbolTable.size; i++) {
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
            return &SymbolTable.entries[i];
        }
    }

    return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    printf("s:%s, valueType:%d\n",s, type);
    if(findSymbol(s) != 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;

    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].link = link;
    
    return &SymbolTable.entries[index];
}

struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}



void deltuplePass(struct nodeType *node){
    if(node->nodeType == NODE_DEL){
      node->parent->child = node->child;
      node->child ->parent = node->parent;
      
      if(node->rsibling != node){
        struct nodeType* RHS = node->rsibling;
        RHS->lsibling = node->child;
        node->child->rsibling = RHS;
      }
      if(node->lsibling!=node){
        struct nodeType *LHS = node->lsibling;
        LHS->rsibling = node->child;
        node->child->lsibling = LHS;
      } 
    }
    
    
    struct nodeType *child = node->child;
    if(child != 0) {
        do {
            deltuplePass(child);
            child = node->child;
            child = child->rsibling;
        } while(child != node->child);
    }



}
void tuplePass(struct nodeType *node){
    
    switch(node->nodeType){
      case NODE_TUPLE:{
        struct nodeType *it = node;
        struct nodeType *RHS;
        int count = 1;
          RHS = it->rsibling;
          do{
            //if(RHS->nodeType==NODE_TUPLE)
            count ++;
            RHS = RHS->rsibling;
          }while(RHS!=it);
          printf("Tuple count:%d\n",count);
          //if(count ==0){
          //}

        break;
      }
      case NODE_TUPLE_HEAD:{
        struct nodeType *it = node;
        struct nodeType *RHS;
        int count = 1;
          RHS = it->rsibling;
          do{
            if(RHS->nodeType==NODE_TUPLE)
              count ++;
            RHS = RHS->rsibling;
          }while(RHS!=it);
          printf("tuple head count:%d\n",count);
          
          if(count ==1){
            node->nodeType = NODE_DEL;
            
            //deleteNode(it);
          }

        break;
      }
      default:{
        struct nodeType *child = node->child;
        if(child != 0) {
          do {
            tuplePass(child);
            child = child->rsibling;
          } while(child != node->child);
        }
      }
  }
}

void semanticCheck(struct nodeType *node) {
 //   printf("nodetype:%d\n", node->nodeType);
    switch(node->nodeType){
      case NODE_FUNC:
      {
        
        if(!findSymbol(node->string)){
          int vtype = node->child->rsibling->child->child->rsibling->valueType;
          addVariable(node->string, vtype, node);
        }
        break;
      }

      case NODE_PATTERN:
      {
        
        struct nodeType *child = node->child;
        struct nodeType *idNode = child;
        do{
          idNode=child;
          do{
            if(idNode->nodeType==NODE_TOKEN){
              if(!findSymbol(idNode->string)){
                addVariable(idNode->string,node->nodeType,node);
              }
            }
            idNode = idNode->rsibling;
          }while(idNode!=child);
          child = child->child;

        }while(child!=NULL);
        break;
      }
    }
    
    /* Default action for other nodes not listed in the switch-case */
    struct nodeType *child = node->child;
    if(child != 0) {
        do {
            semanticCheck(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}

