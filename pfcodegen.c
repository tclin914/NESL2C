#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "symtab.h"
#include <assert.h>
#include "node.h"
#include "codegen.h"

void pfcodegen(FILE *fptr, struct nodeType* node){
  struct nodeType *child = node->child;
  switch(node->nodeType){
  case NODE_NESL:{

    for(int i =0; i<node->counts ; i++){
      if(child->nodeType == NODE_DATATYPE){
        assert(0);// not implement;
      }
      child = child->rsibling;
    }
    for(int i =0 ; i< node-> counts ; i++ ){
      if(child->nodeType == NODE_FUNC){
        pfcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    break;
  }

  case NODE_FUNC:{
    struct SymTableEntry * entry = findSymbol(node->child->table, node->string);
    assert(entry);
    switch(entry->type){
    case TypeInt:
      fprintf(fptr, "int %s", node->string);
      pfcodegen(fptr,node->child); //input parameter
      fprintf(fptr, "{\n");
      dumpTable(fptr, node->child);

      pfcodegen(fptr,node->child->rsibling->rsibling);
      fprintf(fptr, "\n}\n");
      fprintf(fptr, "\n");
      break;
    case TypeSEQ_I:
      fprintf(fptr, "struct Sequence  %s", node->string);
      //pfcodegen(fptr,node->child);
      printparam(fptr, node->child);
      fprintf(fptr, "{\nstruct Sequence res;\n");
      fprintf(fptr, "struct Sequence tmp;\n");

      // FIXME how to dump table without the inputparam
      //dumpTable(fptr, node->child);

      pfcodegen(fptr, node->child->rsibling->rsibling);
      fprintf(fptr, "res = tmp;\nreturn res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct TypeTuple %s", node->string);
      pfcodegen(fptr,node->child);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple res;\n");
      fprintf(fptr, "struct TypeTuple tmp;\n");
      pfcodegen(fptr,node->child->rsibling->rsibling);
      fprintf(fptr, "\n}\n");
      break;
    default:
      assert(0);//not implement;
      break;
    }
    break;
  }

  case NODE_IFELSE:{
    struct nodeType* ifstmt = node->child;
    struct nodeType* thstmt = node->child->rsibling;
    struct nodeType* elstmt = node->child->rsibling->rsibling;
    pfcodegen(fptr,ifstmt);
    pfcodegen(fptr,thstmt);
    pfcodegen(fptr,elstmt);
    break;
  }

  case NODE_IFSTMT:
    fprintf(fptr, "if");
    pfcodegen(fptr, node->child);
    break;

  case NODE_THENSTMT:
    fprintf(fptr, "{\n");
    fprintf(fptr, "tmp = ");
    pfcodegen(fptr, node->child);
    fprintf(fptr, ";\n}");
    break;

  case NODE_ELSESTMT:
    fprintf(fptr, "else{\n");
    pfcodegen(fptr, node->child);
    fprintf(fptr, "}\n");
    break;

  case NODE_LET:
    dumpTable(fptr, node);
    pfcodegen(fptr, node->child);
    fprintf(fptr, "\ntmp = ");
    pfcodegen(fptr, node->child->rsibling);
    fprintf(fptr, ";\n");
    break;

  case NODE_BIND:{
    struct nodeType* child = node->child;
    if(child){
      do{
        pfcodegen(fptr, child);
        child = child->rsibling;
      }while(child!=node->child);
    }  
    break;
  }
  case NODE_EXP:
  case NODE_PATTERN:
    pfcodegen(fptr, node->child);
    break;
  case NODE_OP:
    switch(node->op){
    case OP_LT:
      pfcodegen(fptr, node->child);
      fprintf(fptr, " < ");
      pfcodegen(fptr, node->child->rsibling);
      break;

    case OP_GT:
      pfcodegen(fptr, node->child);
      fprintf(fptr, " > ");
      pfcodegen(fptr, node->child->rsibling);
      break;

    case OP_EQ:
      pfcodegen(fptr, node->child);
      fprintf(fptr, " == ");
      pfcodegen(fptr, node->child->rsibling);
      break;

    case OP_SHARP:
      pfcodegen(fptr, node->child);
      fprintf(fptr, ".len");
      break;

    case OP_BIND:

      pfcodegen(fptr, node->child);
      switch(node->child->rsibling->nodeType){
      case NODE_APPLYBODY1:
      case NODE_APPLYBODY2:
      case NODE_APPLYBODY3:
      case NODE_APPLYBODY4:
        fprintf(fptr, ".ptr = ");
        pfcodegen(fptr, node->child->rsibling);
        break;
      default :
        fprintf(fptr, " = ");
        pfcodegen(fptr, node->child->rsibling);
        fprintf(fptr, ";\n");
      }
      break;

    case OP_PP:
      fprintf(fptr, "concat(");
      pfcodegen(fptr, node->child);
      fprintf(fptr, ",");
      pfcodegen(fptr, node->child->rsibling);
      fprintf(fptr, ")");
      break;
    case OP_DIV:
      pfcodegen(fptr, node->child);
      fprintf(fptr, "/");
      pfcodegen(fptr, node->child->rsibling);
      break;
    }
    break;

  case NODE_SEQ_REF:
    pfcodegen(fptr, node->child);
    fprintf(fptr, ".ptr[");
    pfcodegen(fptr, node->child->rsibling);
    fprintf(fptr, "]");
    break;

  case NODE_APPLYBODY1:
    abort();
    break;

  case NODE_APPLYBODY2:
    pfcodegen(fptr, node->child->rsibling);
    //APP2printFor(fptr, node->child, node->child->rsibling);
    //fprintf(fptr, "for loop");
    
    
    break;

  case NODE_APPLYBODY3:
    pfcodegen(fptr, node->child);
    //APP3printFor(fptr, node->child, node->child->rsibling);
    //fprintf(fptr, "for loop");

    break;

  case NODE_APPLYBODY4:
    break;

  case NODE_RBINDS:
    //FIXME might have many children
    pfcodegen(fptr, node->child);
    break;

  case NODE_IN:
    //FIXME here only consider the simple ApplyBody Exp.
    assert(node->child->nodeType == NODE_TOKEN);
    //assert(node->child->rsibling->nodeType == NODE_TOKEN);
    switch(node->child->rsibling->nodeType){
    case NODE_TOKEN:{
      struct SymTableEntry *entry = 
        findSymbol(node->table, node->child->rsibling->string);
      assert(entry);
      switch(entry->type){
      case TypeSEQ_I:
        fprintf(fptr, "malloc(sizeof(int)*%s.len);\n",entry->name);
        break;
      case TypeSEQ_F:
        fprintf(fptr, "malloc(sizeof(float)*%s.len);\n",entry->name);
        break;
      case TypeSEQ_B:
        fprintf(fptr, "malloc(sizeof(bool)*%s.len);\n",entry->name);
        break;
      case TypeSEQ_C:
        fprintf(fptr, "malloc(sizeof(char)*%s.len);\n",entry->name);
        break;
      case TypeSEQ:
        fprintf(fptr, "malloc(sizeof(%s.ptr[0])*%s.len);\n",
                entry->name,entry->name);
        break;
      default:
        assert(0); //break;
        break;
      }
      break;
    }

    case NODE_NEW_SEQ:
      switch(node->valueType){
      case TypeSEQ:
        fprintf(fptr, "malloc(sizeof(%s)*%d);\n",
                node->child->rsibling->child->string,
                node->child->rsibling->counts);
        //FIXME problems in the future.
        fprintf(fptr, "%s.len = %d;\n",
                node->parent->parent->rsibling->child->string,
                node->child->rsibling->counts);
        break;

      }
      break;
    case NODE_FUNC_CALL:
      break; 
    default:
      abort();
      break;
    }
    break;

  case NODE_TUPLE:
    pfcodegen(fptr, node->child);
    fprintf(fptr, " , ");
    pfcodegen(fptr, node->child->rsibling);
    break;

  case NODE_PAIR:
    fprintf(fptr, "(");
    assert(child);
    //pfcodegen(fptr,node->child);
    do{
      pfcodegen(fptr,node->child);
      child = child->rsibling;
    }while(child!=node->child);
    fprintf(fptr, ")");
    break;
  case NODE_INT:
    fprintf(fptr," %d ",node->iValue);
    break;

  case NODE_FUNC_CALL:{
    if(!strcmp(node->child->string, "rand")){
      fprintf(fptr, "rand"); 
    }else if(!strcmp(node->child->string, "dist")){
      fprintf(fptr, "dist"); 
    }else if(!strcmp(node->child->string, "time")){
      fprintf(fptr, "time"); 
    }else{
      pfcodegen(fptr, node->child);
    }
    pfcodegen(fptr, node->child->rsibling);
    break;
  }

  case NODE_TOKEN:
    switch(node->tokenType){
    case TOKE_ID:{
      // FIXME 0215 if I dump the table in the first, then I don't need
      // to worried whether here is the declaration or assignment.
      struct SymTableEntry* entry = findSymbol(node->table, node->string);
      assert(entry);
      fprintf(fptr, "%s", node->string);
    }

    }
    break;
  default:
    break;
  }
  return;
}


