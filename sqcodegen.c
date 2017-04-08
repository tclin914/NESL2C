#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"
#include "codegencheck.h"
#include "sqcodegen.h"


struct RefTable refTable;
int issrand;

void sqcodegen(FILE *fptr, struct nodeType* node){
  struct nodeType *child = node->child;
  switch(node->nodeType){
  case NODE_NESL:{
    
    for(int i =0; i<node->counts ; i++){
      if(child->nodeType != NODE_DATATYPE && child->nodeType!= NODE_FUNC){
        printGlobalVar(fptr, child); 
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
    
    fprintf(fptr, "#pragma pf global parallel\n");
    fprintf(fptr, "void myFunc1(){\n");
    dumpTable(fptr, node);
    for(int i =0;i<node->counts;i++){
      if(child->nodeType !=NODE_DATATYPE && child->nodeType != NODE_FUNC){
        sqcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    fprintf(fptr, "}\n\n");
    fprintf(fptr, "int main(){\n");
    if(issrand)
      fprintf(fptr, "srand(time(0));\n");
    fprintf(fptr,"myFunc1();\nreturn 1;\n}\n");
    
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
    case TypeFloat:
      fprintf(fptr, "float %s", node->string);
      printparam(fptr, node->child);
      fprintf(fptr, "{\nfloat _res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
      sqcodegen(fptr,node->child->rsibling->rsibling);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
    break;
    case TypeSEQ:
    case TypeSEQ_I:
      fprintf(fptr, "struct Sequence  %s", node->string);
      //sqcodegen(fptr,node->child);
      printparam(fptr, node->child);
      fprintf(fptr, "{\nstruct Sequence _res;\n");
//      fprintf(fptr, "struct Sequence tmp;\n");

      sqcodegen(fptr, node->child->rsibling->rsibling);
      DECREF(fptr, refTable.size);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct TypeTuple %s", node->string);
      sqcodegen(fptr,node->child);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple _res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
      sqcodegen(fptr,node->child->rsibling->rsibling);
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple_IF:
      fprintf(fptr, "struct tupleIF %s", node->string);
      printparam(fptr, node->child);
      fprintf(fptr, "{\nstruct tupleIF _res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
      sqcodegen(fptr,node->child->rsibling->rsibling);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "return _res;\n");
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
      fprintf(fptr, "_res = ");
    sqcodegen(fptr, node->child);
    fprintf(fptr, ";\n}");
    break;

  case NODE_ELSESTMT:
    fprintf(fptr, "else{\n");
    sqcodegen(fptr, node->child);
    DECREF(fptr,refTable.size);
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
  case NODE_LETRET:
    //FIXME  dirtly bypass op_pp 
    if(node->child->op != OP_PP){
      if(node->isEndofFunction){
        if(node->valueType<=TypeTuple_I){
        fprintf(fptr,"_res = ");
        sqcodegen(fptr, node->child);
        fprintf(fptr,";\n");
        }
        else {
          sqcodegen(fptr, node->child);
        }
      }
    }
    else
      sqcodegen(fptr,node->child);
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
  case NODE_OP:{
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
   case OP_LE:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " <= ");
      sqcodegen(fptr, node->child->rsibling);
      break;
    case OP_GE:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " >= ");
      sqcodegen(fptr, node->child->rsibling);
      break; 
    case OP_EQ:
      sqcodegen(fptr, node->child);
      fprintf(fptr, " == ");
      sqcodegen(fptr, node->child->rsibling);
      break;
    case OP_ADD:{
      struct nodeType *op1 = node->child;
      struct nodeType *op2 = op1->rsibling;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      while(op2->nodeType == NODE_PAIR) op2=op2->child;
      fprintf(fptr,"{\n");
      printparam(fptr, node);// declare int add; etc...
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "int op1,op2;\n");
      break;
      case TypeFloat:
        fprintf(fptr, "float op1,op2;\n");
      break;
      }
      switch(op1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
          fprintf(fptr, "op1=");
          sqcodegen(fptr, op1);
          fprintf(fptr, ";\n");
          break;
      break;
      case NODE_TOKEN:
          assert(op1->string);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      }
      switch(op2->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
        fprintf(fptr, "op2=");
        sqcodegen(fptr, op2);
        fprintf(fptr, ";\n");
      break;
      case NODE_TOKEN:
          assert(op2->string);
          fprintf(fptr, "op2=%s;\n",op2->string);
      break;
      default:
        assert(op2->string);
        sqcodegen(fptr, op2);
        fprintf(fptr, "op2=%s;\n", op2->string);
      break;
      }
      fprintf(fptr, "%s = op1 + op2;\n",node->string);
      fprintf(fptr,"}\n");
      if(node->parent->nodeType == NODE_NESL){
        switch(node->valueType){
        case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
        case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
       } 
      }
      break;
      }
    case OP_SUB:
      {
      struct nodeType *op1 = node->child;
      struct nodeType *op2 = op1->rsibling;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      while(op2->nodeType == NODE_PAIR) op2=op2->child;
      fprintf(fptr,"{\n");
      printparam(fptr, node);// declare int add; etc...
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "int op1,op2;\n");
      break;
      case TypeFloat:
        fprintf(fptr, "float op1,op2;\n");
      break;
      }
      switch(op1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
          fprintf(fptr, "op1=");
          sqcodegen(fptr, op1);
          fprintf(fptr, ";\n");
          break;
      break;
      case NODE_TOKEN:
          assert(op1->string);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      }
      switch(op2->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
        fprintf(fptr, "op2=");
        sqcodegen(fptr, op2);
        fprintf(fptr, ";\n");
      break;
      case NODE_TOKEN:
          assert(op2->string);
          fprintf(fptr, "op2=%s;\n",op2->string);
      break;
      default:
        assert(op2->string);
        sqcodegen(fptr, op2);
        fprintf(fptr, "op2=%s;\n", op2->string);
      break;
      }
      fprintf(fptr, "%s = op1 - op2;\n",node->string);
      fprintf(fptr,"}\n");
      if(node->parent->nodeType == NODE_NESL){
        switch(node->valueType){
        case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
        case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
       } 
      }
      break;
      }
    case OP_MUL:{
      struct nodeType *op1 = node->child;
      struct nodeType *op2 = op1->rsibling;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      while(op2->nodeType == NODE_PAIR) op2=op2->child;
      fprintf(fptr,"{\n");
      printparam(fptr, node);// declare int add; etc...
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "int op1,op2;\n");
      break;
      case TypeFloat:
        fprintf(fptr, "float op1,op2;\n");
      break;
      }
      switch(op1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
          fprintf(fptr, "op1=");
          sqcodegen(fptr, op1);
          fprintf(fptr, ";\n");
          break;
      break;
      case NODE_TOKEN:
          assert(op1->string);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      }
      switch(op2->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
        fprintf(fptr, "op2=");
        sqcodegen(fptr, op2);
        fprintf(fptr, ";\n");
      break;
      case NODE_TOKEN:
          assert(op2->string);
          fprintf(fptr, "op2=%s;\n",op2->string);
      break;
      default:
        assert(op2->string);
        sqcodegen(fptr, op2);
        fprintf(fptr, "op2=%s;\n", op2->string);
      break;
      }
      fprintf(fptr, "%s = op1 * op2;\n",node->string);
      fprintf(fptr,"}\n");
      if(node->parent->nodeType == NODE_NESL){
        switch(node->valueType){
        case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
        case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
       } 
      }
      break;
      }    
      case OP_DIV:{
      struct nodeType *op1 = node->child;
      struct nodeType *op2 = op1->rsibling;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      while(op2->nodeType == NODE_PAIR) op2=op2->child;
      fprintf(fptr,"{\n");
      printparam(fptr, node);// declare int add; etc...
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "int op1,op2;\n");
      break;
      case TypeFloat:
        fprintf(fptr, "float op1,op2;\n");
      break;
      }
      switch(op1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
          fprintf(fptr, "op1=");
          sqcodegen(fptr, op1);
          fprintf(fptr, ";\n");
          break;
      break;
      case NODE_TOKEN:
          assert(op1->string);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1=%s;\n",op1->string);
      break;
      }
      switch(op2->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
        fprintf(fptr, "op2=");
        sqcodegen(fptr, op2);
        fprintf(fptr, ";\n");
      break;
      case NODE_TOKEN:
          assert(op2->string);
          fprintf(fptr, "op2=%s;\n",op2->string);
      break;
      default:
        assert(op2->string);
        sqcodegen(fptr, op2);
        fprintf(fptr, "op2=%s;\n", op2->string);
      break;
      }
      fprintf(fptr, "%s = op1 / op2;\n",node->string);
      fprintf(fptr,"}\n");
      if(node->parent->nodeType == NODE_NESL){
        switch(node->valueType){
        case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
        case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
       } 
      }
      break;}    
    case OP_SHARP:
      if(node->child->valueType >= TypeSEQ_I){
      sqcodegen(fptr, node->child);fprintf(fptr, ".len");}
      else assert(0); // not implement yet
      break;

    case OP_BIND:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;

      //sqcodegen(fptr, node->child);
      switch(RHS->nodeType){
        case NODE_APPLYBODY1:
        case NODE_APPLYBODY3:
        case NODE_APPLYBODY4:
          sqcodegen(fptr, RHS);
        break;
        case NODE_APPLYBODY2:
          fprintf(fptr, "//BIND->APPBODY2\n");
          sqcodegen(fptr, RHS);
          fprintf(fptr, "//end of BIND->APPBODY2\n");
          
        break;
        case NODE_FUNC_CALL:
          
          if(!strcmp(RHS->child->string, "time")){
            fprintf(fptr, "{\nclock_t _t1,_t2;\nfloat diff;\n");
            fprintf(fptr, "_t1 = clock();\n");
            fprintf(fptr, "%s = ", LHS->child->child->child->string);
            sqcodegen(fptr, RHS->child->rsibling->child);
            fprintf(fptr, ";\n");
            fprintf(fptr, "_t2 = clock();\n");
            fprintf(fptr, "diff = ((float)(_t2 - _t1) / CLOCKSPEED);\n");
            fprintf(fptr, "tm = diff;\n");
            fprintf(fptr, "}\n");
          }
        break;
        case NODE_TUPLE:
          assert(0); // not likely happened.
        break;
        case NODE_TOKEN:
          if(RHS->valueType >= TypeTuple_I){
            assert(RHS->string);
            struct SymTableEntry *entry = findSymbol(node->table, RHS->string);
            assert(entry);
            //fprintf("%s = %s;\n", 
            printBindTuple(fptr, LHS, entry->link);
          }
          else{
          sqcodegen(fptr,LHS);
          fprintf(fptr, " = ");
          sqcodegen(fptr, RHS);
          fprintf(fptr, ";\n");
          }
        break;
        case NODE_PAIR:{
          fprintf(fptr,"pair\n");
        break;}
        default :
          sqcodegen(fptr, LHS);
          fprintf(fptr, " = ");
          sqcodegen(fptr, RHS);
          fprintf(fptr, ";\n");
      }
      //switch(LHS->nodeType){
      //case NODE_TOKEN:
      //  if(LHS->string)
      //    fpritnf(fptr, "%s = ");
      //}
      break;
    }// end of OP_BIND;
    case OP_PP:
      // if child is a variable, it will be process here.
      if(node->child->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child->rsibling);
      
      // FIXME node->parent is not general
      if(!node->inserttmp){
        fprintf(fptr, "CONCAT(%s, %s, _res, ",
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
        case TypeSEQ:
          switch(node->typeNode->valueType){
            case TypeTuple_F:
              fprintf(fptr, "struct Pair_F, PAIR_F);\n");
            break;
          }
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
      printAddREF(fptr, node->string, node->valueType, node);
       // insertREF(node->string, node->valueType, node);
      }
      break;
    }// end of switch(node->op)
    break;
    }// end of NODE_OP
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
  case NODE_SEQ_TUPLE:{
    struct nodeType *LHS=node->child;
    struct nodeType *RHS = node->child->rsibling;
    switch(LHS->nodeType){
      case NODE_INT:
        fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
                LHS->iValue, node->string, LHS->paramcount);
      break;
    }
    switch(RHS->nodeType){
    case NODE_SEQ_TUPLE:
      sqcodegen(fptr, RHS);
    break;
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
             RHS->iValue, node->string, RHS->paramcount);
    }
  break;
  }
  case NODE_SRCARR:{
    struct nodeType* child = node->child;
    int count=0;
    //count the childs who is SEQ of SEQ. 
    // this is for later refTable use.
    do{
      count++;
      sqcodegen(fptr, child);
      child=child->rsibling;
    }while(child!=node->child);
    node->counts = count;
    node->string = malloc(sizeof(char)*100);
    strcpy(node->string, child->string);

  break;}
  case NODE_NEW_SEQ:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    int deccount =0; 
    fprintf(fptr, "MALLOC(%s, %d, struct Sequence);\n",node->string, node->counts);
    //fprintf(fptr, "for(i =0 ;i<%s.len;i++){\n", node->string);
  
    switch(LHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", LHS->iValue, node->string, LHS->paramcount);
    break;
    case NODE_TOKEN:
      switch(LHS->valueType){
      case TypeSEQ_I:
       fprintf(fptr,"SET_ELEM_SEQ_I(%s,%s,%d);\n", 
                LHS->string, node->string, LHS->paramcount);
        //insertREF(LHS->string,TypeSEQ_I, LHS);
      break;
      default:
        assert(0);
      break;
      }
    break;
    }
    switch(RHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", RHS->iValue, node->string, RHS->paramcount); 
    break;
    case NODE_TOKEN:
      case TypeSEQ_I:
        fprintf(fptr,"SET_ELEM_SEQ_I(%s,%s,%d);\n", 
                RHS->string, node->string, RHS->paramcount);
        //insertREF(RHS->string, TypeSEQ_I, RHS);
      break;
    break;
    case NODE_SEQ_TUPLE:
      sqcodegen(fptr, RHS);
    break;
    }
    
    if(LHS->valueType>=TypeSEQ_I)
      //DECREF(fptr,node->counts);
    printAddREF(fptr, node->string, node->valueType, node);
  break;
  }// end of NEW_SEQ


  case NODE_APPLYBODY1:
    abort();
    break;
  case NODE_APPRET:{
    //don't consider tuple here.
    switch(node->valueType){
      case TypeSEQ_I:
        assert(0);
      break;
      default :
        assert(0);
      break;
    }
  break;
  }
  case NODE_ACTION_TUPLE:{
    assert(0); 
  break;
  }
  case NODE_APPLYBODY2:{
    struct nodeType *LHS = node->child;
    struct nodeType *RBINDS = LHS->rsibling;
    struct nodeType *rbchild = RBINDS->child;
    struct nodeType *varchild = rbchild->child;
    struct nodeType *arrchild = varchild->rsibling;
    int refaddcount=0;
    int forlooprefaddcount=0;

    //open scope
    fprintf(fptr, "{\n");
    dumpTable(fptr, node->child);

    /*generate the src arrays.*/
    refaddcount = refTable.size;
      //sqcodegen(fptr, SRCARR);
    do{
      arrchild = rbchild->child->rsibling;
      sqcodegen(fptr, arrchild);
      rbchild = rbchild->rsibling;
    }while(rbchild!=RBINDS->child);
    
    refaddcount = refTable.size-refaddcount; 
    
    /* allocate the dest array.*/
    fprintf(fptr, "MALLOC(%s,%s.len,struct Sequence);\n",node->string, arrchild->string);
    
    /* generate for loop */
    fprintf(fptr, "_len = %s.len;\n",node->string);
    fprintf(fptr, "#pragma pf parallel_rr\n");
    fprintf(fptr, "for (_i =0; _i <_len;_i++){\n");
    
    /* get elem from src array.*/
    do{
      varchild = rbchild->child;
      arrchild = rbchild->child->rsibling;
      switch(varchild->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I");
      break;
      case TypeSEQ_I:
        fprintf(fptr, "GET_ELEM_SEQ_I");
      break;
      case TypeTuple_F:
        fprintf(fptr, "GET_ELEM_PAIR_F");
      break;
      default:
      assert(0);
      break;
      }
      fprintf(fptr, "(%s,%s,_i);\n",varchild->string,arrchild->string);
      rbchild= rbchild->rsibling;
    }while(rbchild!=RBINDS->child);

      fprintf(fptr, "%s = ", LHS->string);
      sqcodegen(fptr, LHS);
      fprintf(fptr, ";\n");
      
      if(LHS->valueType >=TypeSEQ_I&&LHS->valueType<=TypeSEQ){
        LHS->typeNode = node;
        printAddREF(fptr, LHS->string, LHS->valueType, LHS);
        forlooprefaddcount++;
      }
      switch(LHS->valueType){
      case TypeInt:
        fprintf(fptr, "SET_ELEM_I");
      break;
      case TypeFloat:
        fprintf(fptr, "SET_ELEM_F");
      break;
      case TypeSEQ_I:
        fprintf(fptr, "SET_ELEM_SEQ_I");

      break;
      default:
      assert(0);
      break;
      }
      fprintf(fptr, "(%s,%s,_i);\n", LHS->string, node->string);
      
      DECREF(fptr,forlooprefaddcount);
    fprintf(fptr, "}\n");// close for
    
    if((node->parent->nodeType !=NODE_OP)&&(node->parent->nodeType !=NODE_PAIR)){
    switch(node->valueType){
    case TypeTuple_IF:
      fprintf(fptr, "print_Tuple(%s, I, F);\n", node->string);
      break;
    case TypeInt:
      fprintf(fptr, "print_I(%s);\n", node->string);
      break;
    case TypeFloat:
      fprintf(fptr, "print_I(%s);\n", node->string);
      break;
    case TypeSEQ_I:
      fprintf(fptr, "print_SEQ_I(%s);\n", node->string);
    }
    }
    DECREF(fptr,refaddcount);
    node->typeNode = node->child;
    printAddREF(fptr, node->string, node->valueType, node);
    fprintf(fptr, "}\n");//close scope

    break;
  }
  case GEN_APP3:{
    struct nodeType *APP3 = node->child->rsibling;
    struct nodeType *RBINDS = APP3->child;
    struct nodeType *rbchild = RBINDS->child;
    struct nodeType *FREVAR = rbchild->child;
    struct nodeType *SRCARR = FREVAR->rsibling;
    struct nodeType *FILTER = RBINDS->rsibling; // TODO remove the pair in yacc.
    
            //do{}while(rbchild!=RBINDS->child); //this is for future FILTER_2.

    fprintf(fptr, "FILTER_1(%s, %s,",node->string, FREVAR->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "%s, %s, ", 
        SRCARR->string, 
        FREVAR->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "(");
    sqcodegen(fptr,FILTER->child);
    fprintf(fptr, "));\n");

    printAddREF(fptr, node->string, node->valueType, node);

    break;
  } // end of GEN_APP3

  case NODE_APPLYBODY3:{
    struct nodeType *RBINDS = node->child;
    struct nodeType *rbchild = RBINDS->child;
    struct nodeType *FREVAR = rbchild->child;
    struct nodeType *SRCARR = FREVAR->rsibling;
    struct nodeType *FILTER = RBINDS->rsibling; // TODO remove the pair in yacc.
    
    fprintf(fptr, "FILTER_1(%s, %s,",node->string, FREVAR->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "%s, %s, ", 
        SRCARR->string, 
        FREVAR->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
      break;
    }
    fprintf(fptr, "(");
    sqcodegen(fptr,FILTER->child);
    fprintf(fptr, "));\n");
  break;
  }

  case NODE_APPLYBODY4:
    break;

  case NODE_RBINDS:
    //FIXME might have many children
    sqcodegen(fptr, node->child);
    break;

  case NODE_IN:
    //FIXME here only consider the simple ApplyBody Exp.
    //assert(node->child->nodeType == NODE_TOKEN);
    //assert(node->child->rsibling->nodeType == NODE_TOKEN);
   
    sqcodegen(fptr, node->child->rsibling);
//    node->string
    break;

  case NODE_TUPLE:
    if(node->isEndofFunction){
      //use res
      if(node->child->nodeType == NODE_SEQ_REF)
        sqcodegen(fptr, node->child);
      fprintf(fptr, "_res.a = %s;\n",node->child->string);
      if(node->child->rsibling->nodeType == NODE_TOKEN)
        fprintf(fptr, "_res.b = %s;\n", node->child->rsibling->string);  
    }
    else{
      //use node->string "tmpn"
      if(node->child->nodeType == NODE_SEQ_REF)
        sqcodegen(fptr, node->child);
      fprintf(fptr, "%s.a = %s;\n",node->string, node->child->string);
      if(node->child->rsibling->nodeType == NODE_TOKEN)
        fprintf(fptr, "%s.b = %s;\n", node->string, 
                                      node->child->rsibling->string);  
    }
    break;

  case NODE_PAIR:
    if(node->child->nodeType==NODE_TUPLE){
      if(node->isEndofFunction)
        node->child->isEndofFunction = node->isEndofFunction;
      sqcodegen(fptr, node->child);
    }else{
      fprintf(fptr, "(");
      assert(child);
      //sqcodegen(fptr,node->child);
      do{
        sqcodegen(fptr,node->child);
        child = child->rsibling;
      }while(child!=node->child);
      fprintf(fptr, ")");
    }
    break;
  case NODE_INT:
    fprintf(fptr," %d ",node->iValue);
    break;
  case NODE_FLOAT:
    fprintf(fptr," %f ",node->rValue);
    break;

  case NODE_FUNC_CALL:{

    if(node->parent->nodeType == NODE_NESL){
      fprintf(fptr, "{\n");
      dumpTable(fptr,node);
      //printparam(fptr, node);
      //fprintf(fptr, ";\n");
      switch(node->child->rsibling->child->nodeType){
      case NODE_NEW_SEQ:
        sqcodegen(fptr,node->child->rsibling->child);
      if(node->string){
        if(!strcmp(node->child->string, "rand")){
          fprintf(fptr, "rand"); 

        }else if(!strcmp(node->child->string, "dist")){
          //fprintf(fptr, "dist"); 
        }else if(!strcmp(node->child->string, "time")){
          fprintf(fptr, "time"); 
        }else{
          //so far specialized for qs_test.nesl
          fprintf(fptr, "%s = %s(%s);\n",node->string, node->child->string,
                  node->child->rsibling->child->string);
          //sqcodegen(fptr, node->child);
        }
      }
      break;
      case NODE_TUPLE:
        //printTupleParam(fptr, node->child->rsibling->child); 
      break;
      default:
      if(node->string){
        if(!strcmp(node->child->string, "rand")){
          fprintf(fptr, "rand"); 

        }else if(!strcmp(node->child->string, "dist")){
          //fprintf(fptr, "dist"); 
        }else if(!strcmp(node->child->string, "time")){
          fprintf(fptr, "time"); 
        }else{
          fprintf(fptr, "%s = ",node->string);
          sqcodegen(fptr, node->child);
        }
      } 
      sqcodegen(fptr, node->child->rsibling);
      fprintf(fptr, ";\n");
      break;
      }
      switch(node->valueType){
      case TypeTuple_IF:
        fprintf(fptr, "print_Tuple(%s, I, F);\n", node->string);
        break;
      case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
      case TypeFloat:
        fprintf(fptr, "print_I(%s);\n", node->string);
      break;
      case TypeSEQ_I:
        fprintf(fptr, "print_SEQ_I(%s);\n", node->string);
      }
      DECREF(fptr,refTable.size);
      fprintf(fptr, "}\n");
    }else{
      if(!strcmp(node->child->string, "rand")){
        if(node->valueType ==TypeInt)
        fprintf(fptr, "RAND_I"); 
        else 
        fprintf(fptr, "RAND_F");
        sqcodegen(fptr, node->child->rsibling);
      }else if(!strcmp(node->child->string, "dist")){
        //dist
        struct nodeType* param1 = node->child->rsibling->child->child;
        struct nodeType* param2 = node->child->rsibling->child->child->rsibling;

        fprintf(fptr, "NESLDIST(%s,",node->string);
        switch(param1->nodeType){
          case NODE_TOKEN:
          fprintf(fptr,"%s,",param1->string);
          break;
          case NODE_INT:
          fprintf(fptr,"%d,",param1->iValue);
          break;
          default:
          assert(0);
        }
        switch(param2->nodeType){
          case NODE_TOKEN:
          fprintf(fptr,"%s);\n",param2->string);
          break;
          case NODE_INT:
          fprintf(fptr,"%d);\n",param2->iValue);
          break;
          default:
          assert(0);
        }
        printAddREF(fptr, node->string, TypeSEQ_I, node);
      }else if(!strcmp(node->child->string, "time")){
        fprintf(fptr, "time"); 
      }else if(!strcmp(node->child->string, "plusp")){
        fprintf(fptr, "plusp");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        sqcodegen(fptr, node->child);
        sqcodegen(fptr, node->child->rsibling);
      }
      //sqcodegen(fptr, node->child->rsibling);
      
    }
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


