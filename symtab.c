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


void printNESL(struct nodeType *node){
  switch(node->nodeType){
    case NODE_LIST:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child);
          printf("\n");
          child = child->rsibling;
        }while( child!=node->child);
      }
      break;
    }
    case NODE_FUNC:{
      struct nodeType *child = node->child;
      printf("function %s",node->string);
      printNESL(child);
      printf(":\n");
      printNESL(child->rsibling);
      printf("=\n");
      printNESL(child->rsibling->rsibling);
      printf(";\n");
      break;
    }
    case NODE_TUPLE:{
     struct nodeType *child = node->child;
      printNESL(child);
      child = child->rsibling;
      while(child!=node->child){
        printf(",");
        printNESL(child);
        child = child->rsibling;
      }
      break;
    }
    case NODE_TYPE_PAIR:{
      struct nodeType *child = node->child;

      printf("(");
      printNESL(child);
      child = child->rsibling;
      while(child!=node->child){
        printf(",");
        printNESL(child);
        child = child->rsibling;
      }
      printf(")");
      break;
    }
    case NODE_OP:{
      switch(node->op){
        case OP_BIND:
          printNESL(node->child);
          printf("=");
          printNESL(node->child->rsibling);
          break;
        case OP_ADD:
          printNESL(node->child);
          printf(" + ");
          printNESL(node->child->rsibling);
          break;
        case OP_SUB:
          printNESL(node->child);
          printf("-");
          printNESL(node->child->rsibling);
          break;
        case OP_MUL:
          printNESL(node->child);
          printf("*");
          printNESL(node->child->rsibling);
          break;
        case OP_DIV:
          printNESL(node->child);
          printf("/");
          printNESL(node->child->rsibling);
          break;
        case OP_RARROW:
          printNESL(node->child);
          printf("->");
          printNESL(node->child->rsibling);
          break;
        case OP_LT:
          printNESL(node->child);
          printf("<");
          printNESL(node->child->rsibling);
          break;
        case OP_AT:
          printf("@");
          printNESL(node->child);
          break;
        case OP_UMINUS:
          printf("-");
          printNESL(node->child);
          break;
        case OP_SHARP:
          printf("#");
          printNESL(node->child);
          break;
        
        case OP_GT:     printNESL(node->child); printf(">"); printNESL(node->child->rsibling);  break;
        case OP_EQ:     printNESL(node->child); printf(" == "); printNESL(node->child->rsibling);  break;
        case OP_GE:     printNESL(node->child); printf(" >= "); printNESL(node->child->rsibling);  break;
        case OP_LE:     printNESL(node->child); printf(" <= "); printNESL(node->child->rsibling);  break;
        case OP_NE:     printNESL(node->child); printf(" != "); printNESL(node->child->rsibling);  break;
        case OP_NOT:    printNESL(node->child); printf(" NOT "); printNESL(node->child->rsibling); break;
        case OP_OR:     printNESL(node->child); printf(" OR "); printNESL(node->child->rsibling);  break;
        case OP_COMMA:  printNESL(node->child); printf(" , "); printNESL(node->child->rsibling); break;
        case OP_AND:    printNESL(node->child); printf(" AND "); printNESL(node->child->rsibling);  break;
        case OP_NOR:    printNESL(node->child); printf(" NOR "); printNESL(node->child->rsibling);  break;
        case OP_NAND:   printNESL(node->child); printf(" NAND "); printNESL(node->child->rsibling); break;
        case OP_XOR:    printNESL(node->child); printf(" XOR "); printNESL(node->child->rsibling);  break;
        case OP_LARROW: printNESL(node->child); printf(" <- "); printNESL(node->child->rsibling);  break;
        case OP_UPT:    printNESL(node->child); printf(" ^ "); printNESL(node->child->rsibling);  break;
        case OP_PP:     printNESL(node->child); printf(" ++ "); printNESL(node->child->rsibling); break;




      }
      break;
    }
    case NODE_TOKEN:{
      switch(node->tokenType){
        case TOKE_ID:
          printf("%s",node->string);
          break;
        case TOKE_INT:
          printf("INT");
          break;
        case TOKE_FLOAT:
          printf("FLOAT");
          break;
        
      
        
      }
      break;
    }
    case NODE_LET:{
      printf("let ");
      printNESL(node->child);
      printf("in ");
      printNESL(node->child->rsibling);
      //printf(";\n");
      break; 
    } 
    case NODE_PATTERN:
    case NODE_EXP:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child);
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_BIND:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child);
          printf(";\n");
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_FUNC_CALL:{
      printNESL(node->child);
      //printf("(");
      printNESL(node->child->rsibling);
      //printf(")");

      break;
    }
    case NODE_FLOAT:{
      printf("%f",node->rValue);
      break;
    }
    case NODE_INT:{
      printf("%d",node->iValue);
      break;
    }

    case NODE_IN:{
      printNESL(node->child);
      printf(" IN ");
      printNESL(node->child->rsibling);
      break;
    }
    case NODE_APPLYBODY1:{
      printf("{ ");
      printNESL(node->child);
      printf("}");
      break;
    }
    case NODE_APPLYBODY2:{
      printf("{ ");
      printNESL(node->child);
      printf(" : ");
      printNESL(node->child->rsibling);
      printf("}");
      
      break;
    }
    case NODE_APPLYBODY3:{
      printf("{ ");
      printNESL(node->child);
      printf(" | ");
      printNESL(node->child->rsibling);
      printf("}");
      
      break;
    }
    case NODE_APPLYBODY4:{
      printf("{ ");
      printNESL(node->child);
      printf(" : ");
      printNESL(node->child->rsibling);
      printf(" | ");
      printNESL(node->child->rsibling->rsibling);
      printf("}");
      
      break;
    }
    case NODE_TYPE_SEQ:
    case NODE_SEQ:{
      struct nodeType *child = node->child;
      printf("[");
      if(child!=0){
        printNESL(child);
        child = child->rsibling;
        while(child!=node->child){
          printf(", ");
          printNESL(child);
          child = child->rsibling;
        }
      }
      printf("]");
      break;
    }
    case NODE_IFELSE:{
      printNESL(node->child);
      printNESL(node->child->rsibling);
      printNESL(node->child->rsibling->rsibling);
      break;
    }
    case NODE_IFSTMT:{
      printf("if ");
      printNESL(node->child);
      break;
    }
    case NODE_THENSTMT:{
      printf("then ");
      printNESL(node->child);
      printf("\n");
      break;
    }
    case NODE_ELSESTMT:{
      printf("else \n");
      printNESL(node->child);
      break;
    }
    case NODE_RBINDS:{
      struct nodeType * child = node->child;
      if(child!=0){
        printNESL(child);
        child=child->rsibling;
        while(child!=node->child){
          printf(";");
          printNESL(child);

          child = child->rsibling;
        }
      }
      break;
    }
    case NODE_SEQ_REF:{
      printNESL(node->child);
      printf("[");
      printNESL(node->child->rsibling);
      printf("]");
      break;
    }
    case NODE_FILTER:{
      printNESL(node->child);
      break;
    }
    case NODE_STRING:{
      printf("%s", node->string);
      break;
    }

    
  
  }// End of Switch

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

