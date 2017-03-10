#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"
#include "pfcodegen.h"
#include "sqcodegen.h"

#define MAX 10

struct RefTable refTable;
int elmindex[MAX];
int tmpindex[MAX];


void sqcodegen(FILE *fptr, struct nodeType* node){
  struct nodeType *child = node->child;
  switch(node->nodeType){
  case NODE_NESL:{
    
    for(int i =0; i<node->counts ; i++){
      if(child->nodeType != NODE_DATATYPE && child->nodeType!= NODE_FUNC){
        phase1(fptr, child); 
      }
      child = child->rsibling;
    }
    
    for(int i =0; i<node->counts ; i++){
      if(child->nodeType == NODE_DATATYPE){
        assert(0);// not implement;
      }
      child = child->rsibling;
    }
    
    for(int i =0 ; i< node-> counts ; i++ ){
      if(child->nodeType == NODE_FUNC){
        sqcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    
    fprintf(fptr, "#pragma global parallel\n");
    fprintf(fptr, "void myFunc1(){\n");
    for(int i =0;i<node->counts;i++){
      if(child->nodeType !=NODE_DATATYPE && child->nodeType != NODE_FUNC){
        sqcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    fprintf(fptr, "}\n\n");
    fprintf(fptr, "int main(){\nmyFunc1();\nreturn1;\n}\n");
    
    break;
  }

  case NODE_FUNC:{
    struct SymTableEntry * entry = findSymbol(node->child->table, node->string);
    assert(entry);
    if(node->isparallel_rr)
      fprintf(fptr, "#pragma pf device parallel\n");
    else 
      fprintf(fptr, "#pragma pf device inline\n");
    switch(entry->type){
    case TypeInt:
      fprintf(fptr, "int %s", node->string);
      sqcodegen(fptr,node->child); //input parameter
      fprintf(fptr, "{\n");
      dumpTable(fptr, node->child);

      sqcodegen(fptr,node->child->rsibling->rsibling);
      fprintf(fptr, "\n}\n");
      fprintf(fptr, "\n");
      break;
    case TypeSEQ_I:
      fprintf(fptr, "struct Sequence  %s", node->string);
      //sqcodegen(fptr,node->child);
      printparam(fptr, node->child);
      fprintf(fptr, "{\nstruct Sequence res;\n");
//      fprintf(fptr, "struct Sequence tmp;\n");

      sqcodegen(fptr, node->child->rsibling->rsibling);
      fprintf(fptr, "return res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct TypeTuple %s", node->string);
      sqcodegen(fptr,node->child);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
      sqcodegen(fptr,node->child->rsibling->rsibling);
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple_IF:
      fprintf(fptr, "struct TypeTuple_IF %s", node->string);
      sqcodegen(fptr,node->child);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple_IF res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
      sqcodegen(fptr,node->child->rsibling->rsibling);
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
    sqcodegen(fptr,ifstmt);
    sqcodegen(fptr,thstmt);
    sqcodegen(fptr,elstmt);
    break;
  }

  case NODE_IFSTMT:
    fprintf(fptr, "if");
    sqcodegen(fptr, node->child);
    break;

  case NODE_THENSTMT:
    fprintf(fptr, "{\n");
    if(node->isEndofFunction)
      fprintf(fptr, "res = ");
    sqcodegen(fptr, node->child);
    fprintf(fptr, ";\n}");
    break;

  case NODE_ELSESTMT:
    fprintf(fptr, "else{\n");
    sqcodegen(fptr, node->child);
    fprintf(fptr, "}\n");
    if(node->isEndofFunction)
      //fprintf(fptr, "endofFunction\n");
    break;

  case NODE_LET:
    dumpTable(fptr, node);
    sqcodegen(fptr, node->child);
    //fprintf(fptr, "\ntmp = ");
    sqcodegen(fptr, node->child->rsibling);
    //fprintf(fptr, ";\n");
    break;

  case NODE_BIND:{
    struct nodeType* child = node->child;
    if(child){
      do{
        sqcodegen(fptr, child);
        child = child->rsibling;
      }while(child!=node->child);
    }  
    break;
  }
  case NODE_EXP:
  case NODE_PATTERN:
    sqcodegen(fptr, node->child);
    break;
  case NODE_OP:
    switch(node->op){
    case OP_LT:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " < ");
      sqcodegen(fptr, node->child->rsibling);
      break;

    case OP_GT:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " > ");
      sqcodegen(fptr, node->child->rsibling);
      break;

    case OP_EQ:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " == ");
      sqcodegen(fptr, node->child->rsibling);
      break;

    case OP_SHARP:
      sqcodegen(fptr, node->child);
      fprintf(fptr, ".len");
      break;

    case OP_BIND:

      sqcodegen(fptr, node->child);
      switch(node->child->rsibling->nodeType){
      case NODE_APPLYBODY1:
      case NODE_APPLYBODY2:
      case NODE_APPLYBODY3:
      case NODE_APPLYBODY4:
        fprintf(fptr, ".ptr = ");
        sqcodegen(fptr, node->child->rsibling);
        break;
      default :
        fprintf(fptr, " = ");
        sqcodegen(fptr, node->child->rsibling);
        fprintf(fptr, ";\n");
      }
      break;

    case OP_PP:
      // if child is a variable, it will be process here.
      if(node->child->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child->rsibling);
      
      // FIXME node->parent is not general
      if(!node->inserttmp){
        fprintf(fptr, "CONCAT(%s, %s, res, ",
              node->child->string,
              node->child->rsibling->string
        );
      }
      else{
      fprintf(fptr, "CONCAT(%s, %s, %s, ",
              node->child->string,
              node->child->rsibling->string,
              node->string);
      }
      switch(node->valueType){
        case TypeSEQ_I:
          fprintf(fptr, "int, I);\n");
          break;
        default:
          assert(0);//not implemented.
          break;
      }
      if(node->inserttmp){
        switch(node->valueType){
          case TypeSEQ_I:
            break;
          default:
            assert(0);//not implemented.
            break;
        }
        insertREF(node->string, node->valueType, node);
      }
      break;
    case OP_DIV:
      sqcodegen(fptr, node->child);
      fprintf(fptr, "/");
      sqcodegen(fptr, node->child->rsibling);
      break;
    }
    break;
  case NODE_SEQ_REF:{
    switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,",node->string, node->child->string);
        sqcodegen(fptr, node->child->rsibling); // print index.
        fprintf(fptr, ");\n");
        break;
      case TypeSEQ_I:
        fprintf(fptr, "GET_ELEM_SEQ_I(%s,%s,",node->string, node->child->string);
        sqcodegen(fptr, node->child->rsibling); // print index.
        fprintf(fptr, ");\n");
        break;
      default:
        assert(0);
    }
    break;
  }
  case GEN_SEQ_REF:
    //sqcodegen(fptr, node->child);
    switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,",node->string, node->child->rsibling->child->string);
        sqcodegen(fptr, node->child->rsibling->child->rsibling); // print index.
        fprintf(fptr, ");\n");
        break;
      default:
        assert(0);//not implemented.
        break;
    }
    break;

  case NODE_APPLYBODY1:
    abort();
    break;

  case NODE_APPLYBODY2:
    sqcodegen(fptr, node->child->rsibling);
    //APP2printFor(fptr, node->child, node->child->rsibling);
    //fprintf(fptr, "for loop");
    
    break;
  case GEN_APP2:{
    struct nodeType* phase1 = node->child->rsibling->child->rsibling->child;
    struct nodeType *phase2 = node->child->child;
    struct nodeType* phase3 = node->child->rsibling->child;

    if(phase1->nodeType == NODE_IN){
      if(phase1->child->rsibling->nodeType == NODE_NEW_SEQ){
        int count = phase1->child->rsibling->counts;
        assert(count);
        if(count == 2){
          fprintf(fptr, "MAKE_SEQ_2(%s, ",phase1->string);
          switch(phase1->child->rsibling->child->valueType){
            case TypeSEQ_I:
              fprintf(fptr, "struct Sequence, SEQ_I, ");
              break;
          }
          fprintf(fptr, "%s,%s);\n", 
            phase1->child->rsibling->child->string,
            phase1->child->rsibling->child->rsibling->string);  
          
          switch(phase1->valueType){
            case TypeSEQ:
              break;
          }
          insertREF(phase1->string, phase1->valueType, phase1);
          
          fprintf(fptr, "MALLOC(%s, 2, ",phase2->string);
          switch(phase2->valueType){
            case TypeSEQ:
            case TypeSEQ_I:
            case TypeSEQ_B:
            case TypeSEQ_F:
            case TypeSEQ_C:
              fprintf(fptr, "struct Sequence);\n");
          }

          //TODO phase 3 forloop and set REFCNT
          if(node->isparallel_rr){
            fprintf(fptr, "#pragma pf parallel_rr\n");
            fprintf(fptr, "for(i=0;i<%d;i++){\n", phase1->child->rsibling->counts);
            dumpTable(fptr, phase3);
            
            // phase3 is action node
            // phase3->rsibling is RBINDS
            switch(phase3->nodeType){
              case NODE_FUNC_CALL:{
                struct SymTableEntry *paramEntry = 
                  findSymbol(phase3->table, phase3->child->rsibling->child->string);
                switch(paramEntry->type){
                  case TypeSEQ_I:
                    fprintf(fptr, "GET_ELEM_SEQ_I(%s, %s, i);\n",
                      phase3->child->rsibling->child->string,
                      phase1->string);
                    if(phase3->isEndofFunction){
                      fprintf(fptr, "res = %s", phase3->child->string);
                      sqcodegen(fptr, phase3->child->rsibling);
                      fprintf(fptr, ";\n");
                      // int because paramtype is TypeSEQ_I
                      fprintf(fptr, "SET_ELEM_SEQ_I(res, %s, i);\n", phase2->string);
                      fprintf(fptr, "}\n");
                    }
                    
                    // struct sequence since it's TypeSEQ_I
                    // indicate that parent is TypeSEQ_SEQ_I
                    insertREF(phase2->string, phase2->valueType, phase2);
                    deleteREF(phase1->string);
                    break;
                }
                break;
              }
              default:
                assert(0);//not implement;
                break;
            }
            fprintf(fptr, "}\n");
          }
        }
      }
    } 
    if(phase3->nodeType == NODE_FUNC_CALL){
    }
  break;
  }
  case GEN_APP3:{
    fprintf(fptr, "FILTER_1(%s, %s,",node->string, node->child->rsibling->child->child->child->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "%s, %s, ", 
        node->child->rsibling->child->child->child->rsibling->string,
        node->child->rsibling->child->child->child->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "(");
    sqcodegen(fptr, node->child->rsibling->child->rsibling->child);
    fprintf(fptr, "));\n");
    switch(node->valueType){
      case TypeSEQ_I:
        //fprintf(fptr, "int), 1);\n");
        break;
    }
    insertREF(node->string, node->valueType, node);
    break;
  }
  case NODE_APPLYBODY3:
    sqcodegen(fptr, node->child);
    //APP3printFor(fptr, node->child, node->child->rsibling);
    //fprintf(fptr, "for loop");

    break;

  case NODE_APPLYBODY4:
    break;

  case NODE_RBINDS:
    //FIXME might have many children
    sqcodegen(fptr, node->child);
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
    sqcodegen(fptr, node->child);
    fprintf(fptr, " , ");
    sqcodegen(fptr, node->child->rsibling);
    break;

  case NODE_PAIR:
    fprintf(fptr, "(");
    assert(child);
    //sqcodegen(fptr,node->child);
    do{
      sqcodegen(fptr,node->child);
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
      sqcodegen(fptr, node->child);
    }
    sqcodegen(fptr, node->child->rsibling);
    if(node->parent->nodeType == NODE_NESL)
      fprintf(fptr, ";\n");
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


