#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}

//struct SymTable SymbolTable;
struct SymTable * newSymTable(struct SymTable * parent){
  struct SymTable *table = (struct SymTable*)malloc(sizeof(struct SymTable));
  table->parent = parent;
  return table;
}

struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s) {
    //struct SymTableEntry * entry;
    if(s==0)
      return 0;//FIXME return error.

    for(int i=0; i<SymbolTable.size; i++) {
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
            return &SymbolTable.entries[i];
        }
    
    }
    if(SymbolTable->parent !=0)
      return findSymbol(SymTable->parent, s);  
    else
      return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    struct SymTable *SymbolTable = link->SymTable;
    
    printf("s:%s, valueType:%d\n",s, type);
    if(findSymbol(link->SymTable, s) != 0) {
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

void tupleTransform(struct nodeType *node){
  struct nodeType * foo  = node->child->lsibling;
  if(foo->nodeType == NODE_TUPLE){
    struct nodeType *c1 = nthChild(1,foo);
    struct nodeType *c2 = nthChild(2,foo);
    c1->parent = node;
    c2->parent = node;
    c1->rsibling = c2;
    c2->lsibling = c1;
    c2->rsibling = node->child;
    node->child->lsibling = c2;
    c1->lsibling = foo->lsibling;
    foo->lsibling->rsibling = c1;
    tupleTransform(node);
  }
}

void removePair(struct nodeType *node){
  struct nodeType *child = node->child;
  struct nodeType *RHS = node->rsibling;
  struct nodeType *LHS = node->lsibling;
  int counts=0;
  int hasTuple=0;
  int i =0;
  switch(node->nodeType){
    case NODE_SEQ:
      if(child!=0){
        do{
          if(child->nodeType == NODE_TUPLE)
            hasTuple =1;
          counts++;
          child = child->rsibling;
        }while(child!=node->child);
      } 
      if(hasTuple){
          tupleTransform(node);
      }
      printf("SEQ:count:%d\n",counts);
      
      break;
    default:
      if(child!=0){
        do{
          removePair(child);
          child = child->rsibling;
        }while(child != node->child);
      }
      break;
  }
}


void typeBinding(struct nodeType *node1, struct nodeType *node2){

  if(node2->nodeType == NODE_TYPE_PAIR){
    node2= node2->child;
    typeBinding(node1, node2);
  }

  else if(node1->nodeType == NODE_TUPLE && node2->nodeType == NODE_TUPLE){
    struct nodeType * child1,*child2;
    child1 = node1->child;
    child2 = node2->child;
    if(node1->child!=0 && node2->child!=0){
      do{
        typeBinding(child1, child2);
        child1 = child1->rsibling;
        child2 = child2->rsibling;
      }while((child1!=node1->child) && (child2!=node2->child));
    }
  }

  else if (node1->nodeType == NODE_TOKEN && node2->nodeType == NODE_TOKEN){
    node1->valueType = node2->valueType;
    printf("node1:%s, type:%s\n", node1->string,node2->string);
    
    struct nodeType * RHS1,*RHS2;
    RHS1 = node1->rsibling;
    RHS2 = node2->rsibling;
    if(RHS1!=node1 && RHS2 != node2){
      //typeBinding(RHS1,RHS2);
    }
  }
  else if(node1->nodeType == NODE_TOKEN && node2->nodeType==NODE_TUPLE){
    node1->nodeType = TypeTuple1;
    node1->tuplenode = node2;
    //addVariable(
    printf("node1:%s, type:%s\n", node1->string,"tuple");

  }
}

void typeCheck(struct nodeType *node){
  switch(node->nodeType){
  case NODE_LIST:{
    struct nodeType *child = node->child;
    if(child!=0){
      do{
        typeCheck(child);
        child = child->rsibling;
      }while( child!=node->child);
    }
    break;
  }
  case NODE_FUNC:{
    struct nodeType *child = node->child;

    //TODO 
    // FIXME change the yacc.y stuff , parse typedeclare the same way as usual.
    struct nodeType *inputParam = node->child->child;
    struct nodeType *typeDef = node->child->rsibling->child;
    struct nodeType *functionType = typeDef->rsibling;
    //typeBinding(inputParam, typeDef);
    if(typeDef->childcount == inputParam->childcount){
      typeBinding(inputParam, typeDef);
    }

    // input param
    typeCheck(child);

    // input type
    typeCheck(child->rsibling);

    // function body.
    typeCheck(child->rsibling->rsibling);
    break;
  }

  case NODE_TUPLE:{
    struct nodeType *child = node->child;
    struct nodeType *child2 = node->child->rsibling;
    typeCheck(child);
    child = child->rsibling;
    while(child!=node->child){
      typeCheck(child);
      child = child->rsibling;
    }
    struct nodeType *child1 = node->child;
    int haha = 0;
    haha = child1->valueType *10;
    haha += child2->valueType;
    switch(haha){
      case 0:
        
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;

      case 10:
        break;
      case 11://float,float
        
        break;
      case 12:
        break;
      case 13:
        break;

      case 20:
        break;
      case 21:
        break;
      case 22:
        break;
      case 23:
        break;
    
      case 30:
        break;
      case 31:
        break;
      case 32:
        break;
      case 33:
        break;
    
    }
    if(child1->nodeType == NODE_TOKEN && child2->nodeType==NODE_TOKEN)
    break;
  }

  case NODE_TYPE_PAIR:{
    struct nodeType *child = node->child;

    typeCheck(child);
    child = child->rsibling;
    while(child!=node->child){
      typeCheck(child);
      child = child->rsibling;
    }
    break;
  }
  case NODE_OP:{
    switch(node->op){
    case OP_BIND:

      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      // node->child->valueType = node->child->rsibling->valueType;

      break;
    case OP_ADD:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      // node->child->valueType == 
      break;
    case OP_SUB:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      break;
    case OP_MUL:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      break;
    case OP_DIV:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      break;
    case OP_RARROW:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      break;
    case OP_LT:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);
      break;
    case OP_AT:
      typeCheck(node->child);
      break;
    case OP_UMINUS:
      typeCheck(node->child);
      break;
    case OP_SHARP:
      node->valueType = TypeInt;
      typeCheck(node->child);
      break;

    case OP_GT:     
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  
      break;
    case OP_EQ:
      typeCheck(node->child);
      typeCheck(node->child->rsibling);  
      break;
    case OP_GE:    
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_LE:     
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_NE:     
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_NOT:    
      typeCheck(node->child); 
      typeCheck(node->child->rsibling); break;
    case OP_OR:     
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  
      break;
    case OP_COMMA:  
      typeCheck(node->child); 
      typeCheck(node->child->rsibling); break;
    case OP_AND:    
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_NOR:    
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_NAND:   
      typeCheck(node->child); 
      typeCheck(node->child->rsibling); break;
    case OP_XOR:    
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_LARROW: 
      typeCheck(node->child); 
      typeCheck(node->child->rsibling);  break;
    case OP_UPT:    
      typeCheck(node->child);
      typeCheck(node->child->rsibling);  break;
    case OP_PP:     
      typeCheck(node->child); 
      typeCheck(node->child->rsibling); break;
    }
    break;
  }
  case NODE_TOKEN:{
    switch(node->tokenType){
    case TOKE_ID:
      //addVariable(
      break;
    case TOKE_INT:
      node->valueType = TypeInt;
      break;
    case TOKE_FLOAT:
      node->valueType = TypeFloat;
      break;
    }
    break;
  }
  case NODE_LET:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);
    break; 
  } 
  case NODE_PATTERN:
  case NODE_EXP:{
    struct nodeType *child = node->child;
    if(child!=0){
      do{
        typeCheck(child);
        child = child->rsibling;
      }while(child!=node->child);
    }
    break;
  }
  case NODE_BIND:{
    struct nodeType *child = node->child;
    if(child!=0){
      do{
        typeCheck(child);
        child = child->rsibling;
      }while(child!=node->child);
    }
    break;
  }
  case NODE_FUNC_CALL:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);

    break;
  }
  case NODE_FLOAT:{
    node->valueType = TypeFloat; 
    break;
  }
  case NODE_INT:{
    break;
  }

  case NODE_IN:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);
    break;
  }
  case NODE_APPLYBODY1:{
    typeCheck(node->child);
    break;
  }
  case NODE_APPLYBODY2:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);

    break;
  }
  case NODE_APPLYBODY3:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);

    break;
  }
  case NODE_APPLYBODY4:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);
    typeCheck(node->child->rsibling->rsibling);

    break;
  }
  case NODE_TYPE_SEQ:
  case NODE_SEQ:{
    struct nodeType *child = node->child;
    if(child!=0){
      typeCheck(child);
      child = child->rsibling;
      while(child!=node->child){
        typeCheck(child);
        child = child->rsibling;
      }
    }
    child = node->child;
    if(child->rsibling = child){
      switch(child->valueType){
        case TypeInt:
          node->valueType = TypeSEQ_I;
          break;
        case TypeFloat:
          node->valueType = TypeSEQ_F;
          break;
        case TypeBool:
          node->valueType = TypeSEQ_B;
          break;
        case TypeChar:
          node->valueType = TypeSEQ_C;
          break;
        
      }
    }
    break;
  }
  case NODE_IFELSE:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);
    typeCheck(node->child->rsibling->rsibling);
    break;
  }
  case NODE_IFSTMT:{
    typeCheck(node->child);
    break;
  }
  case NODE_THENSTMT:{
    typeCheck(node->child);
    break;
  }
  case NODE_ELSESTMT:{
    typeCheck(node->child);
    break;
  }
  case NODE_RBINDS:{
    struct nodeType * child = node->child;
    if(child!=0){
      typeCheck(child);
      child=child->rsibling;
      while(child!=node->child){
        typeCheck(child);

        child = child->rsibling;
      }
    }
    break;
  }
  case NODE_SEQ_REF:{
    typeCheck(node->child);
    typeCheck(node->child->rsibling);
    break;
  }
  case NODE_FILTER:{
    typeCheck(node->child);
    break;
  }
  case NODE_STRING:{
    node->valueType = NODE_TYPE_CHAR;
    break;
  }

  }// End of Switch

}// End of typeCheck

void printNESL(struct nodeType *node, FILE* yyout){
  switch(node->nodeType){
    case NODE_LIST:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          fprintf(yyout, "\n");
          child = child->rsibling;
        }while( child!=node->child);
      }
      break;
    }
    case NODE_FUNC:{
      struct nodeType *child = node->child;
      fprintf(yyout,"function %s",node->string);
      //fprintf(yyout,"(");
      printNESL(child, yyout);
      //fprintf(yyout,")");
      fprintf(yyout,":\n");
      printNESL(child->rsibling, yyout);
      fprintf(yyout,"=\n");
      printNESL(child->rsibling->rsibling, yyout);
      fprintf(yyout,";\n");
      break;
    }
    case NODE_TUPLE:{
     struct nodeType *child = node->child;
      //fprintf(yyout,"(");
      printNESL(child, yyout);
      child = child->rsibling;
      while(child!=node->child){
        fprintf(yyout,",");
        printNESL(child, yyout);
        child = child->rsibling;
      }
      //fprintf(yyout,")");
      break;
    }
    case NODE_PAIR:
    case NODE_TYPE_PAIR:{
      struct nodeType *child = node->child;

      fprintf(yyout,"(");
      printNESL(child, yyout);
      child = child->rsibling;
      while(child!=node->child){
        fprintf(yyout,",");
        printNESL(child, yyout);
        child = child->rsibling;
      }
      fprintf(yyout,")");
      break;
    }
    case NODE_OP:{
      switch(node->op){
        case OP_BIND:
          printNESL(node->child, yyout);
          fprintf(yyout,"=");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_ADD:
          printNESL(node->child, yyout);
          fprintf(yyout," + ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_SUB:
          printNESL(node->child, yyout);
          fprintf(yyout,"-");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_MUL:
          printNESL(node->child, yyout);
          fprintf(yyout,"*");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_DIV:
          printNESL(node->child, yyout);
          fprintf(yyout,"/");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_RARROW:
          fprintf(yyout,"(");
          printNESL(node->child, yyout);
          fprintf(yyout,")");
          fprintf(yyout,"->");
          fprintf(yyout,"(");
          printNESL(node->child->rsibling, yyout);
          fprintf(yyout,")");
          break;
        case OP_LT:
          printNESL(node->child, yyout);
          fprintf(yyout,"<");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_AT:
          fprintf(yyout,"@");
          printNESL(node->child, yyout);
          break;
        case OP_UMINUS:
          fprintf(yyout,"-");
          printNESL(node->child, yyout);
          break;
        case OP_SHARP:
          fprintf(yyout,"#");
          printNESL(node->child, yyout);
          break;
        
        case OP_GT:     printNESL(node->child, yyout); fprintf(yyout,">"); printNESL(node->child->rsibling, yyout);  break;
        case OP_EQ:     printNESL(node->child, yyout); fprintf(yyout," == "); printNESL(node->child->rsibling, yyout);  break;
        case OP_GE:     printNESL(node->child, yyout); fprintf(yyout," >= "); printNESL(node->child->rsibling, yyout);  break;
        case OP_LE:     printNESL(node->child, yyout); fprintf(yyout," <= "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NE:     printNESL(node->child, yyout); fprintf(yyout," != "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NOT:    printNESL(node->child, yyout); fprintf(yyout," NOT "); printNESL(node->child->rsibling, yyout); break;
        case OP_OR:     printNESL(node->child, yyout); fprintf(yyout," OR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_COMMA:  printNESL(node->child, yyout); fprintf(yyout," , "); printNESL(node->child->rsibling, yyout); break;
        case OP_AND:    printNESL(node->child, yyout); fprintf(yyout," AND "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NOR:    printNESL(node->child, yyout); fprintf(yyout," NOR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NAND:   printNESL(node->child, yyout); fprintf(yyout," NAND "); printNESL(node->child->rsibling, yyout); break;
        case OP_XOR:    printNESL(node->child, yyout); fprintf(yyout," XOR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_LARROW: printNESL(node->child, yyout); fprintf(yyout," <- "); printNESL(node->child->rsibling, yyout);  break;
        case OP_UPT:    printNESL(node->child, yyout); fprintf(yyout," ^ "); printNESL(node->child->rsibling, yyout);  break;
        case OP_PP:     printNESL(node->child, yyout); fprintf(yyout," ++ "); printNESL(node->child->rsibling, yyout); break;




      }
      break;
    }
    case NODE_TOKEN:{
      switch(node->tokenType){
        case TOKE_ID:
          fprintf(yyout,"%s",node->string);
          break;
        case TOKE_INT:
          fprintf(yyout,"INT");
          break;
        case TOKE_FLOAT:
          fprintf(yyout,"FLOAT");
          break;
        
      
        
      }
      break;
    }
    case NODE_LET:{
      fprintf(yyout,"let ");
      printNESL(node->child, yyout);
      fprintf(yyout,"in ");
      printNESL(node->child->rsibling, yyout);
      //fprintf(yyout,";\n");
      break; 
    } 
    case NODE_PATTERN:
    case NODE_EXP:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_BIND:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          fprintf(yyout,";\n");
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_FUNC_CALL:{
      printNESL(node->child, yyout);
      //fprintf(yyout,"(");
      printNESL(node->child->rsibling, yyout);
      //fprintf(yyout,")");

      break;
    }
    case NODE_FLOAT:{
      fprintf(yyout,"%f",node->rValue);
      break;
    }
    case NODE_INT:{
      fprintf(yyout,"%d",node->iValue);
      break;
    }

    case NODE_IN:{
      printNESL(node->child, yyout);
      fprintf(yyout," IN ");
      printNESL(node->child->rsibling, yyout);
      break;
    }
    case NODE_APPLYBODY1:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout,"}");
      break;
    }
    case NODE_APPLYBODY2:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," : ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_APPLYBODY3:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," | ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_APPLYBODY4:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," : ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout," | ");
      printNESL(node->child->rsibling->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_TYPE_SEQ:
    case NODE_SEQ:{
      struct nodeType *child = node->child;
      fprintf(yyout,"[");
      if(child!=0){
        printNESL(child, yyout);
        child = child->rsibling;
        while(child!=node->child){
          fprintf(yyout,", ");
          printNESL(child, yyout);
          child = child->rsibling;
        }
      }
      fprintf(yyout,"]");
      break;
    }
    case NODE_IFELSE:{
      printNESL(node->child, yyout);
      printNESL(node->child->rsibling, yyout);
      printNESL(node->child->rsibling->rsibling, yyout);
      break;
    }
    case NODE_IFSTMT:{
      fprintf(yyout,"if ");
      printNESL(node->child, yyout);
      break;
    }
    case NODE_THENSTMT:{
      fprintf(yyout," then ");
      printNESL(node->child, yyout);
      fprintf(yyout,"\n");
      break;
    }
    case NODE_ELSESTMT:{
      fprintf(yyout," else \n");
      printNESL(node->child, yyout);
      break;
    }
    case NODE_RBINDS:{
      struct nodeType * child = node->child;
      if(child!=0){
        printNESL(child, yyout);
        child=child->rsibling;
        while(child!=node->child){
          fprintf(yyout,";");
          printNESL(child, yyout);

          child = child->rsibling;
        }
      }
      break;
    }
    case NODE_SEQ_REF:{
      printNESL(node->child, yyout);
      fprintf(yyout,"[");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"]");
      break;
    }
    case NODE_FILTER:{
      printNESL(node->child, yyout);
      break;
    }
    case NODE_STRING:{
      fprintf(yyout,"%s", node->string);
      break;
    }

    
  
  }// End of Switch

}
void semanticCheck(struct nodeType *node) {
 //   printf("nodetype:%d\n", node->nodeType);
    switch(node->nodeType){
      case NODE_FUNC:
      {
        
        if(!findSymbol(node->table, node->string)){
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
              if(!findSymbol(node->table, idNode->string)){
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

