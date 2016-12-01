#include "newnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}
struct SymTable SymbolTable;

void codegen(FILE *fptr, struct nodeType* node){

  struct nodeType *child = node->child;
      switch(node->nodeType){
      case NODE_ASSIGN_STMT:
        {
        struct nodeType* LHS = node->child;
        struct nodeType* RHS = node->child->rsibling;
        codegen(fptr, LHS);
        if(RHS->nodeType == NODE_SEQ)
        {
          struct nodeType* next = RHS;
          int n = RHS->arraydepth;
          do{
            fprintf(fptr, "[%d]",next->counts+1); 
            next = next->arraynext;;
          } while(next!=NULL);
        }
        
        fprintf(fptr, "= ");
        codegen(fptr, RHS);
        fprintf(fptr, ";");
        break;
        }
      case NODE_SYM_REF:
        fprintf(fptr, "int %s ", node->string);
        break;
      case NODE_SEQ:{
        struct nodeType *idNode = node->child;
        
        fprintf(fptr, "{");
        do{
          if(idNode->nodeType == NODE_SEQ)
            codegen(fptr, idNode);
          else{
            fprintf(fptr, "%d", idNode->iValue);
            if(idNode->rsibling != node->child)
              fprintf(fptr, ",");
          }
          idNode = idNode->rsibling;
        }
        while(idNode != node->child);
          fprintf(fptr, "},");
        break;
        }
      default:
        fprintf("this nodeType: %d", node->nodeType);
        break;
      }
  return;
}

struct SymTableEntry* findSymbol(char *s) {
    for(int i=0; i<SymbolTable.size; i++) {
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
            return &SymbolTable.entries[i];
        }
    }

    return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    printf("s:%s, Type:%d\n",s, type);
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



void semanticCheck(struct nodeType *node) {
 //   printf("nodetype:%d\n", node->nodeType);
    switch(node->nodeType){
      case NODE_FUNC:
      {
        if(!findSymbol(node->string)){
          addVariable(node->string,node->nodeType,node);
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

