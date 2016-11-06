#include "node.h"
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
    switch(node->nodeType) {
        case NODE_SYM_REF:
        {
          if(!findSymbol(node->string)){
            addVariable(node->string, node->nodeType, node);
          }
        }
        case NODE_INT: {
            node->valueType = TypeInt;
            return;
        }

        case NODE_FLOAT: {
            node->valueType = TypeReal;
            return;
        }

        case NODE_OP:{
          return;
        }
        case NODE_SEQ:{
          struct nodeType *idNode = node->child;
          int count = 0;
          int depth = 0;
          
          do{
            count++;
            if(idNode->nodeType == NODE_SEQ){
              semanticCheck(idNode);
              if(depth < idNode->arraydepth){
                depth = idNode->arraydepth;
                node->arraynext = idNode;
                }
            }
            idNode = idNode->rsibling;
          }
          while(idNode != node->child);
          node->counts = count;
          node->arraydepth = depth+1;
          printf("node_counts:%d, node_depth:%d\n",node->counts,node->arraydepth);
          return;
        }

        case NODE_ASSIGN_STMT: {
            struct nodeType *child1 = nthChild(1, node);
            struct nodeType *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);

            if(child1->string != NULL && child2->nodeType==NODE_SEQ)
            {
              child2->string = (char*)malloc(child1->string);
              strcpy(child2->string, child1->string);
            }
              
            /* We only implement the checking for integer and real types
               you should implement the checking for array type by yourself */
            if(child1->valueType != child2->valueType) {
                if(node->nodeType == NODE_OP)
                    printf("Error: type mismatch for operator\n");
                else{
                    printf("Error: type mismatch for assignment\n");
                    printf("type: %d, %d\n",child1->valueType,child2->valueType);
                    }
                exit(0);
            }
            node->valueType = child1->valueType;
            return;
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

