#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}


struct SymTable * rootTable;
struct SymTable * newSymTable(struct SymTable * parent){
  struct SymTable *table = (struct SymTable*)malloc(sizeof(struct SymTable));
  table->parent = parent;
  return table;
}

struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s) {
    //struct SymTableEntry * entry;
    if(s==0)
      return 0;//FIXME return error.

    for(int i=0; i<SymbolTable->size; i++) {
        if(strcmp(s, SymbolTable->entries[i].name) == 0) {
            return &SymbolTable->entries[i];
        }
    
    }
    if(SymbolTable->parent !=0)
      return findSymbol(SymbolTable->parent, s);  
    else
      return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    struct SymTable *SymbolTable = link->table;
    
    printf("s:%s, valueType:%d\n",s, type);
    if(findSymbol(link->table, s) == 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        exit(0);
    }

    int index = SymbolTable->size;
    (SymbolTable->size)++;

    strcpy(SymbolTable->entries[index].name, s);
    SymbolTable->entries[index].type = type;
    SymbolTable->entries[index].link = link;
    
    return &SymbolTable->entries[index];
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
  
  // doing the tupleTransform
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

  if(node1->nodeType == NODE_TYPE_PAIR){
    node1= node1->child;
    typeBinding(node1, node2);
    return;
  }

  if(node2->nodeType == NODE_TYPE_PAIR){
    node2= node2->child;
    typeBinding(node1, node2);
    return ;
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
  printf("typeBinding done\n");
}

void semanticPass( struct nodeType *node){
  // create and set a table in rootNode. 
  // then start the Analysis
  // which consist of table insertion, 
  // typeBinding and typeChechking.
  rootTable = (struct SymTable*)malloc(sizeof(struct SymTable));
  setTable(node, rootTable);
  typeAnalysis(node);
}

void typeAnalysis( struct nodeType *node){
  switch(node->nodeType){
    case NODE_LIST:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          typeAnalysis(child);
          child = child->rsibling; 
        }while(child!=node->child);
      }
      break;
    }
      
    case NODE_FUNC:{
     
      if(node->child->rsibling->op == OP_RARROW){
        // Bind the inputParameter with TypeDeclaration
        typeBinding(node->child, node->child->rsibling->child);

        // Analyse the returnType of the function, RHS of op_rarrow.
        typeAnalysis(node->child->rsibling->child->rsibling);

        // Assign the returnType to the functionNode
        typeAnalysis(node->child->rsibling->rsibling);
        switch(node->child->rsibling->rsibling->valueType){
          case TypeInt:
          case TypeBool:
          case TypeFloat:
          case TypeChar:
            break;
          case TypeSEQ_I:
          case TypeSEQ_F:
          case TypeSEQ_B:
          case TypeSEQ_C:
            break;
        }
        // Add the function node into SymbolTable
        addVariable(node->string, TypeInt, node);  
      }
      else {
        printf("error in node_func of semanticPass\n");
        return;
      } 
     break;
    }
    case NODE_IFELSE:{
      struct nodeType* ifstmt = node->child;
      struct nodeType* thstmt = node->child->rsibling;
      struct nodeType* elstmt = node->child->rsibling->rsibling;
      typeAnalysis(ifstmt);
      typeAnalysis(thstmt);
      typeAnalysis(elstmt);
      
      // assert(ifstmt->valueType, TypeBool); 
      // assert(thstmt->valueType, elstmt->valueType);
      // assert(thstmt->valueType, function type);
      node->nodeType = elstmt->nodeType;
      
      break;
    }
    
    case NODE_OP:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      typeAnalysis(LHS);
      typeAnalysis(RHS);
      // assert(LHS->nodeType, RHS->nodeType);
      // 
      switch(node->op){
        case OP_BIND:
          
          break;
        case OP_ADD:
          break;
        case OP_SUB:
          break;
        case OP_MUL:
          break;
        case OP_DIV:
          break;
        case OP_RARROW:
          break;
        case OP_LT:
          break;
        case OP_AT:
          break;
        case OP_UMINUS:
          break;
        case OP_SHARP:
          break;
        case OP_GT:
        case OP_EQ:
        case OP_GE:
        case OP_LE:
        case OP_NE:
          node->nodeType = TypeBool;
          break;
        case OP_NOT:
          break;
        case OP_OR:
          break;
        case OP_COMMA:
          break;
        case OP_AND:
          break;
        case OP_NOR:
          break;
        case OP_NAND:
          break;
        case OP_XOR:
          break;
        case OP_LARROW:
          break;
        case OP_UPT:
          break;
        case OP_PP:
          break;
      }// end of node->op
      break;
    } // end of NODE_OP
    
    case NODE_TOKEN: {
        switch(node->tokenType){
          case TOKE_ID:
            if(!findSymbol(node->string, node->table))
              addVariable(node->string, TypeInt, node);
            break;
          case TOKE_INT:
            node->valueType = TypeInt;
            break;
          case TOKE_FLOAT:
            node->valueType = TypeFloat;
            break;
          case TOKE_BOOL:
            node->valueType = TypeBool;
            break;
          case TOKE_CHAR:
            node->valueType = TypeChar;
            break;
          case TOKE_STRING:
            node->valueType = TypeSEQ_C;
            break;
          default:
            break;
        }// end of switch tokenType
      break;
    }
    
  }// end of switch nodeType

}//end of typeAnalysis

//void setTable(struct nodeType *node, struct SymTable *table){
void setTable(struct nodeType *node){
  switch(node->nodeType){
    case NODE_FUNC:
    case NODE_LET:{
      printf("nodetype:%d create scope oh yeah~\n",node->nodeType);
      struct SymTable * newScope = newSymTable(node);
      node->table = newScope;
      break;
    }
    default:
      if(node->parent!=0){
        node->table = node->parent->table;
      }
  }

  struct nodeType * child = node->child;
  if(child!=0){
    do{
      setTable(child);
      child = child->rsibling;
    }while(child!=node->child);
  }
  
}

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

