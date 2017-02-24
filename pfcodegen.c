#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"

#define MAX 10

int elmindex[MAX];
int tmpindex[MAX];
char elm[MAX][5] = {"elm1","elm2","elm3","elm4","elm5","elm6","elm7","elm8","elm9","elm10"};
char tmp[MAX][5] = {"tmp1","tmp2","tmp3","tmp4","tmp5","tmp6","tmp7","tmp8","tmp9","tmp10"};


int insertelm(struct nodeType* node){
  for(int i =0; i<MAX; i++){
    if(elmindex[i] ==0){
      addVariable(elm[i], node->valueType, node);
      elmindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int inserttmp(struct nodeType* node){
  for(int i =0; i<=MAX; i++){
    if(tmpindex[i] ==0){
      addVariable(tmp[i],node->valueType,node);
      tmpindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

/**
* pfcheck is aimed to 
* 1. insert needed variables to symbol table,
* 2. label out the EXP that is return value of function.
* 3. make the etc...
*/
void pfcheck(struct nodeType* node){
  struct nodeType *child = node->child;
  switch(node->nodeType){
    case NODE_IFELSE:{
      if(node->rsibling == node){
        node->child->rsibling->isEndofFunction = 1;
        node->child->rsibling->rsibling->isEndofFunction = 1;
      }
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      pfcheck(node->child->rsibling->rsibling);
      break;
    }
    case NODE_OP:{
      struct nodeType* LHS;
      struct nodeType* RHS;
      LHS = node->child;
      RHS = LHS->rsibling;
      switch(node->op){
        case OP_PP:{
          pfcheck(LHS);
          pfcheck(RHS);
          int index = inserttmp(node);
          assert(index!=-1);
          node->string = malloc(sizeof(char)*100);
          strcpy(node->string, tmp[index]);
          assert(node->string);
          break;
        }  
        case OP_BIND:{
          switch(child->rsibling->nodeType){
            case NODE_APPLYBODY1:
              // not implement
              break;
            case NODE_APPLYBODY2:
              // {action: RBINDS}
              pfcheck(node->child->rsibling);
              node->nodeType = GEN_APP2;
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, child->child->string);
              break;
            case NODE_APPLYBODY3:
              // {RBINDS|FILTER}
              pfcheck(node->child);
              node->nodeType = GEN_APP3;
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, child->child->string);
              break;  
            case NODE_APPLYBODY4:
              // not implemented
              break;
            case NODE_SEQ_REF:
              //  node->nodeType
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, node->child->child->string);
              node->nodeType = GEN_SEQ_REF;
              break;
            default:
              pfcheck(RHS);
              break;
           }
           break;
        }
        default:
          pfcheck(LHS);
          if(LHS->rsibling != LHS)
            pfcheck(LHS->rsibling);
          break;
      }//end of case node->op
      break;
    }
    case NODE_NEW_SEQ:{
      int index = inserttmp(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, tmp[index]);
      assert(node->string);
      
      break;
    }
    case NODE_IN:{
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      if(node->child->rsibling->nodeType == NODE_NEW_SEQ){
        assert(node->child->rsibling->string);
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, node->child->rsibling->string);
        free(node->child->rsibling->string);
      }
      break;
    }
    case NODE_LET:{
      if(node->rsibling == node){
        node->child->rsibling->isEndofFunction = 1;
      }
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      break;
    }
    case NODE_SEQ_REF:{
      int index = insertelm(node);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, elm[index]);
      assert(node->string);
      break;
    }
    case NODE_EXP:{
      // TODO
      if(node->isEndofFunction ==1){
        if(node->child->rsibling == node->child) 
          node->child->isEndofFunction =1;
        else{
         //FIXME Exp
         //       |
         //     BINDS
         //     /   \
         //   ...   ... 
         // as a result:   
         node->child->rsibling->isEndofFunction = 1; 
        }
      
      }
      pfcheck(node->child);
      break; 
    }
    case NODE_RBINDS:
    default:{
      struct nodeType* child = node->child;
      if(child){
        do{
          pfcheck(child);
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
      
  }// end of switch node->nodeType
}

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
      // dumpTable(fptr, node->child);

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
    if(node->isEndofFunction)
      fprintf(fptr, "res = ");
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
    //fprintf(fptr, "\ntmp = ");
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
      if(node->child->nodeType==NODE_OP)
        pfcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType==NODE_OP)
        pfcodegen(fptr, node->child->rsibling);

      fprintf(fptr, "CONCAT(%s,%s,%s,",
              node->child->string,
              node->child->rsibling->string,
              node->string);
      switch(node->valueType){
        case TypeSEQ_I:
          fprintf(fptr, "int, I);\n");
          break;
        default:
          assert(0);//not implemented.
          break;
      }
      
      //fprintf(fptr, ")");
      break;
    case OP_DIV:
      pfcodegen(fptr, node->child);
      fprintf(fptr, "/");
      pfcodegen(fptr, node->child->rsibling);
      break;
    }
    break;

  case GEN_SEQ_REF:
    //pfcodegen(fptr, node->child);
    switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,",node->string, node->child->rsibling->child->string);
        pfcodegen(fptr, node->child->rsibling->child->rsibling); // print index.
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
    pfcodegen(fptr, node->child->rsibling);
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
          
          fprintf(fptr,"atomicAdd(REFCNT(%s, ",phase1->string);
          switch(phase1->valueType){
            case TypeSEQ:
                fprintf(fptr, "struct Sequence), 1);\n");
              break;
          }

          
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
        fprintf(fptr, " int, I,");
      break;
    }
    fprintf(fptr, "%s, %s", 
        node->child->rsibling->child->child->child->rsibling->string,
        node->child->rsibling->child->child->child->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,");
      break;
    }
    fprintf(fptr, "(");
    pfcodegen(fptr, node->child->rsibling->child->rsibling->child);
    fprintf(fptr, "));\n");
    fprintf(fptr, "atomicAdd(REFCNT(%s, ",node->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, "int), 1);\n");
        break;
    }
    break;
  }
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


