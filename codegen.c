#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}

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
        fprintf(fptr, "this nodeType: %d", node->nodeType);
        break;
      }
  return;
}
