#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "symtab.h"
#include <assert.h>
#include "node.h"
#include "codegen.h"
#define help(s) {printf("\thelp: %s\n",s);}


void gentype(FILE* fptr){
  struct nodeType *link;
  for(int i =0; i<typeTable->size;i++){
    link = typeTable->link[i];
    switch(link->valueType){
      case TypeTuple:
      fprintf(fptr, "struct ");
      gentypes(fptr, link);
      fprintf(fptr, " {\n");
      if(link->typeNode->child->valueType == TypeSEQ) 
        fprintf(fptr,"struct Sequence a;\n");
      else {
        printtype(fptr, link->typeNode->child);
        fprintf(fptr, " a;\n");
      }
      if(link->typeNode->child->rsibling->valueType == TypeSEQ)
        fprintf(fptr,"struct Sequence b;\n");
      else {
      printtype(fptr, link->typeNode->child->rsibling);
      fprintf(fptr, " b;\n");
      }
      fprintf(fptr, "};\n");
      break;
      case TypeSEQ:
      break;
      default: assert(0);
    }
  }
}

void printNESL(struct nodeType *node, FILE* yyout){
  switch(node->nodeType){
  case NODE_NESL:
  case NODE_LIST:{
    int count = 0;
    struct nodeType *child = node->child;
    if(child!=0){
      do{
        count++;
        printNESL(child, yyout);
        fprintf(yyout, "\n");
        child = child->rsibling;
      }while( child!=node->child);
    }
    node->counts = count;
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
  case NODE_ASSIGN:{
    printNESL(node->child, yyout);
    fprintf(yyout,"=");
    printNESL(node->child->rsibling, yyout);
    break;
  }
  case NODE_OP:{
    switch(node->op){
    case OP_BIND:
      assert(0);
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
    case OP_GT:     
      printNESL(node->child, yyout); 
      fprintf(yyout,">"); 
      printNESL(node->child->rsibling, yyout);  
      break;
    case OP_EQ:     
      printNESL(node->child, yyout); 
      fprintf(yyout," == "); 
      printNESL(node->child->rsibling, yyout);  
      break;
    case OP_GE:     
      printNESL(node->child, yyout); 
      fprintf(yyout," >= "); 
      printNESL(node->child->rsibling, yyout);  
      break;
    case OP_LE:     
      printNESL(node->child, yyout); 
      fprintf(yyout," <= "); 
      printNESL(node->child->rsibling, yyout); 
      break;
    case OP_NE:     
      printNESL(node->child, yyout);
      fprintf(yyout," != ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_NOT:    
      printNESL(node->child, yyout);
      fprintf(yyout," NOT ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_OR:    
      printNESL(node->child, yyout);
      fprintf(yyout," OR ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_COMMA: 
      printNESL(node->child, yyout);
      fprintf(yyout," , ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_AND:   
      printNESL(node->child, yyout);
      fprintf(yyout," AND ");
      printNESL(node->child->rsibling, yyout);  
      break;
    case OP_NOR:   
      printNESL(node->child, yyout);
      fprintf(yyout," NOR ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_NAND:   
      printNESL(node->child, yyout); 
      fprintf(yyout," NAND ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_XOR:    
      printNESL(node->child, yyout); 
      fprintf(yyout," XOR ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_LARROW: 
      printNESL(node->child, yyout);
      fprintf(yyout," <- ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_UPT:    
      printNESL(node->child, yyout);
      fprintf(yyout," ^ ");
      printNESL(node->child->rsibling, yyout);
      break;
    case OP_PP:     
      printNESL(node->child, yyout); 
      fprintf(yyout," ++ ");
      printNESL(node->child->rsibling, yyout); 
      break;
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
    int counts=0;
    if(child!=0){
      do{
        counts++;
        printNESL(child, yyout);
        fprintf(yyout,";\n");
        child = child->rsibling;
      }while(child!=node->child);
      node->childcount = counts;
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
  case NODE_BOOL:{
    fprintf(yyout,"%s",node->string);
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
  case NODE_NEW_SEQ:
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
}// End of printNESL

void dumpTable(FILE *fptr, struct nodeType* node){
  struct SymTable * table = node->table;
  for(int i = 0; i<table->size ; i++){
    if(table->entries[i].link->nodeType!=NODE_FUNC 
       && table->entries[i].isParam!=1){
      table->entries[i].isParam=1;// ensure every symbol printed onetime.
      switch(table->entries[i].type){
      case TypeInt :
        fprintf(fptr, "int %s;\n",table->entries[i].name);
        break;
      case TypeFloat :
        fprintf(fptr, "float %s;\n",table->entries[i].name);
        break;
      case TypeBool :
        fprintf(fptr, "bool %s;\n",table->entries[i].name);
        break;
      case TypeChar :
        fprintf(fptr, "char* %s = malloc(sizeof(char*100));\n",table->entries[i].name);
        break;
      case TypeSEQ:
        fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
        break;
      case TypeTuple:{
        struct nodeType *link = table->entries[i].link;
        struct nodeType *lchild ;
        struct nodeType *rchild ;
        if(link->valueType>=TypeSEQ) {
          assert(link->typeNode);
          lchild = link->typeNode->child;
          rchild = lchild->rsibling;
        }
        
        //FIXME black magic
        fprintf(fptr, "struct ");
        gentypes(fptr, link);
        fprintf(fptr, " %s;\n", table->entries[i].name);
        break;
        }
      default:
        fclose(fptr);
        assert(0);//not implement;
        break;
      }//end of switch(entry->type)
    }//end of if
  }//end of for.
}// end of dumpTable.

void printType(FILE *fptr, struct nodeType *node){
  switch(node->valueType){
    case TypeInt:
    fprintf(fptr,"I");
    break;
    case TypeFloat:
    fprintf(fptr,"F");
    break;
    case TypeBool:
    fprintf(fptr,"B");
    break;
    case TypeChar:
    fprintf(fptr,"C");
    break;
    case TypeSEQ:
    fprintf(fptr,"SEQ");
    printType(fptr,node->typeNode->child);
    break;
    case TypeTuple:
    fprintf(fptr,"t_");
    printType(fptr, node->typeNode->child);
    printType(fptr, node->typeNode->child->rsibling);
    break;
    default:
    assert(0); //no way~
    break;
  }
}

void printtupleparam(FILE *fptr, struct nodeType* node){
  if(node->nodeType == NODE_TUPLE){
    printtupleparam(fptr, node->child);
    printtupleparam(fptr, node->child->rsibling);

  }else if(node->nodeType == NODE_PAIR){
    printtupleparam(fptr, node->child);
    return;
  }
  else {
    switch(node->valueType){
    case TypeFloat:
      fprintf(fptr, "%f;\n", node->rValue);
      break;
    case TypeInt:
      fprintf(fptr, "%d;\n", node->iValue);
      break;
    case TypeSEQ:
      assert(0); //not implemented;
      break;
    } 
  }
  assert(node->nodeType == NODE_TUPLE); // make sure it's tuple here.
  struct nodeType *LHS = node->child;
  struct nodeType *RHS = node->child->rsibling;
  while(LHS->nodeType == NODE_PAIR)
    LHS=LHS->child;
  while(RHS->nodeType == NODE_PAIR)
    RHS=RHS->child;
  fprintf(fptr, "%s = %s.a;\n", LHS->string, node->string);
  fprintf(fptr, "%s = %s.b;\n", RHS->string, node->string);
  if(LHS->nodeType == NODE_TUPLE)
    printtupleparam(fptr,LHS);
  if(RHS->nodeType == NODE_TUPLE)
    printtupleparam(fptr,RHS);

}


void printparam(FILE *fptr, struct nodeType* node){
  switch(node->nodeType){
  case NODE_TUPLE:
    printparam(fptr, node->child);
    fprintf(fptr, ", ");
    printparam(fptr, node->child->rsibling);
    break;
  case NODE_PAIR:
    fprintf(fptr, "(");
    printparam(fptr, node->child);
    fprintf(fptr, ")");
    break;
  case NODE_FUNC_CALL:
  case NODE_TOKEN:{
    struct SymTableEntry *entry;
    //struct nodeType *refNode = node->typeNode;
    //printparam(fptr, refNode);
    switch(node->valueType){
    case TypeSEQ:
      fprintf(fptr, "struct Sequence ");
      break;
    case TypeInt:
      fprintf(fptr, "int ");
      break;
    case TypeFloat:
      fprintf(fptr, "float ");
      break;
    case TypeBool:
      fprintf(fptr, "bool ");
      break;
    case TypeTuple:{
      struct nodeType *typer = node->typeNode;
      assert(typer);
      fprintf(fptr, "struct ");
      gentypes(fptr, typer);
      break;
    }
    default:
      assert(0); // tuple not implemented.
      break;
    }
    
    fprintf(fptr, " %s", node->string);
    entry = findSymbol(node->table, node->string,REFERENCE);
    assert(entry);
    entry->isParam =1;
    break;
  }
  case NODE_TYPE_SEQ:{
    switch(node->valueType){
    case TypeSEQ:
      fprintf(fptr, "struct Sequence");
      break;
    default :
      //TODO
      break;
    }
    break;
  }
  } 

}

void printBindTuple(FILE *fptr, struct nodeType *node1, struct nodeType *node2){
  assert(node1); assert(node2);
  struct nodeType *child1 = node1->child;
  struct nodeType *child2 = node2->child;
  switch(node1->nodeType){
  case NODE_PATTERN:
    printBindTuple(fptr, child1, node2);
    return;
  case NODE_PAIR:
    printBindTuple(fptr, child1, node2);
    return;

  }
  switch(node2->nodeType){
  case NODE_PATTERN:
    printBindTuple(fptr, node1, child2);
    return;
  case NODE_TYPE_PAIR:
    printBindTuple(fptr, node1, child2);
    return;
  case NODE_PAIR:
    printBindTuple(fptr, node1, child2);
    return;
  }

  switch(node1->nodeType){
  case NODE_TOKEN:
    fprintf(fptr, "%s = ",node1->string);  
    switch(node2->nodeType){
    case NODE_TOKEN: //not likely happened.
      fprintf(fptr, "%s;\n",node2->string);
      break;
    case NODE_TUPLE:
      fprintf(fptr, "%s;\n",node2->string);
      break;
    }
    break;
  case NODE_TUPLE:
    switch(node2->nodeType){
    case NODE_TOKEN:

      assert(node2->valueType == TypeTuple);
      fprintf(fptr, "%s = %s;\n", node1->string, node2->string);

    case NODE_TUPLE:{
      struct nodeType* refnode = newNode(NODE_TUPLE);
      refnode->valueType = node1->valueType;
      refnode->string = malloc(sizeof(char)*100);
      strcpy(refnode->string, node1->string);
      child1 = child1;
      child2 = child1->rsibling;
      while(child1->nodeType==NODE_PAIR)// remove Pair
        child1=child1->child;
      while(child2->nodeType==NODE_PAIR)
        child2=child2->child;

      if(node1->valueType == TypeTuple){
       ; 
      }else{
        fprintf(fptr, "%s = %s.a;\n",child1->string, node1->string);
        fprintf(fptr, "%s = %s.b;\n",child2->string, node1->string);
      }
      if(child1->valueType == TypeTuple){ 
        printBindTuple(fptr, child1, refnode);
      }
      if(child2->valueType == TypeTuple){ 
        printBindTuple(fptr, child2, refnode);
      }
      free(refnode);
      break;}
    }// end of switch node2->nodeType
    break;
  }// end of switch node1->nodeType
}// end of printBindTuple

void gentypes(FILE *fptr, struct nodeType *type){
  switch(type->valueType){
  case TypeInt:
    fprintf(fptr,"I");
    break;
  case TypeFloat:
    fprintf(fptr,"F");
    break;
  case TypeBool:
    fprintf(fptr,"B");
    break;
  case TypeChar:
    fprintf(fptr,"C");
    break;
  case TypeSEQ:
    fprintf(fptr,"S");
    gentypes(fptr,type->typeNode->child); 
    break;
  case TypeTuple:
    fprintf(fptr,"T");
    gentypes(fptr,type->typeNode->child); 
    gentypes(fptr,type->typeNode->child->rsibling); 
    break;
    }
}

void printtype(FILE *fptr, struct nodeType *type){
  switch(type->valueType){
  case TypeInt:
    fprintf(fptr,"int");
    break;
  case TypeFloat:
    fprintf(fptr,"float");
    break;
  case TypeBool:
    fprintf(fptr,"bool");
    break;
  case TypeChar:
    fprintf(fptr,"char");
    break;
  case TypeSEQ:
    fprintf(fptr,"struct Sequence");
    break;
  case TypeTuple:
    fprintf(fptr,"struct T");
    gentypes(fptr,type->typeNode->child); 
    gentypes(fptr,type->typeNode->child->rsibling); 
    break;
  default:
    assert(0); // new type?
  }
}

void printEXPBINDTUPLE(FILE *fptr, struct nodeType* node1, struct nodeType *node2){
  assert(node1->nodeType==NODE_TUPLE);
  assert(node2->string);
  struct nodeType *L1 = node1->child; 
  while(L1->nodeType ==NODE_PAIR) L1=L1->child;
  struct nodeType *R1 = L1->rsibling; 

  // TODO make it more general.
  switch(L1->nodeType){
  case NODE_TOKEN:
    fprintf(fptr,"%s",L1->string); 
  break;
  default:
  abort();
  }
  
  fprintf(fptr,"=%s.a;\n",node1->string);
  switch(R1->nodeType){
  case NODE_TOKEN:
    fprintf(fptr,"%s",R1->string); 
  break;
  default:
  abort();
  }
  fprintf(fptr,"=%s.b;\n",node1->string);
}

int containArray(struct nodeType *node){
  if(node->nodeType == NODE_OP && node->op == OP_SHARP) return  1;
  if(node->valueType<TypeSEQ) return 0;
  else if(node->valueType==TypeSEQ) return 1;
  else if(node->valueType == TypeTuple){
    struct nodeType* Lchild = node->typeNode->child;
    struct nodeType* Rchild = node->typeNode->child->rsibling;
    assert(Lchild);
    assert(Rchild);
    if(containArray(Lchild) || containArray(Rchild))
      return 1;
    else return 0;
  }
  else return 0;
}
