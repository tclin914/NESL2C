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
        //printGlobalVar(fptr, child); 
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
    fprintf(fptr, "SET_HEAP_SIZE(MALLOC_HEAP_SIZE);\n");
    fprintf(fptr,"myFunc1();\nCUDA_ERROR_CHECK();\nreturn 1;\n}\n");

    break;
  }

  case NODE_FUNC:{
    struct SymTableEntry * entry = findSymbol(node->child->table, node->string);
    struct nodeType * parameter = node->child;
    struct nodeType * funcbody = parameter->rsibling->rsibling;

    assert(entry);
    if(node->isparallel_rr)
      fprintf(fptr, "#pragma pf device parallel\n");
    else 
      fprintf(fptr, "#pragma pf device inline\n");

    switch(entry->type){
    case TypeInt:
      fprintf(fptr, "int %s", node->string);
      printparam(fptr, parameter);
      fprintf(fptr, "{\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr,funcbody);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeFloat:
      fprintf(fptr, "float %s", node->string);
      printparam(fptr, parameter);
      fprintf(fptr, "{\nfloat _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr,funcbody);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeSEQ:
    case TypeSEQ_I:
      fprintf(fptr, "struct Sequence  %s", node->string);
      printparam(fptr, parameter);
      fprintf(fptr, "{\nstruct Sequence _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr, funcbody);
      DECREF(fptr, refTable.size);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct TypeTuple %s", node->string);
      sqcodegen(fptr,parameter);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr,funcbody);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple_IF:
      fprintf(fptr, "struct tupleIF %s", node->string);
      printparam(fptr, parameter);
      fprintf(fptr, "{\nstruct tupleIF _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr,funcbody);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple_SF:
      fprintf(fptr, "struct tupleSF %s", node->string);
      printparam(fptr, parameter);
      fprintf(fptr, "{\nstruct tupleSF _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr,funcbody);
      DECREF(fptr,refTable.size);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
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

  case NODE_IFSTMT:{
    struct nodeType * condition = node->child;
    while(condition->nodeType == NODE_PAIR) condition = condition->child;
    sqcodegen(fptr, condition); 
    fprintf(fptr, "if(%s)",condition->string);
    break;
  }
  case NODE_ELSESTMT:
    fprintf(fptr, "else");
  case NODE_THENSTMT:{
    struct nodeType* LHS= node->child;
    fprintf(fptr, "{\n");
    switch(LHS->nodeType){
    case NODE_TOKEN:
      assert(LHS->string);
      fprintf(fptr, "%s = %s;\n",node->string, LHS->string);
      break;
    case NODE_INT:
      fprintf(fptr, "%s = %d;\n", node->string,LHS->iValue);
      break;
    case NODE_FLOAT:
      fprintf(fptr, "%s = %f;\n", node->string,LHS->rValue);
      break;
    case NODE_CHAR:
      fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
      break;
    case NODE_BOOL:
      fprintf(fptr, "%s = %i;\n", node->string,LHS->iValue);
      break;
    default:
      sqcodegen(fptr, LHS);
      assert(LHS->string);
      fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
      break;
    }
    DECREF(fptr,refTable.size);
    fprintf(fptr, "\n}\n");
    break;}

  case NODE_LET:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    fprintf(fptr, "{\n");
    dumpTable(fptr, node);
    sqcodegen(fptr, LHS);
    sqcodegen(fptr, RHS);
    assert(RHS->string);
    DECREF(fptr,refTable.size);
    fprintf(fptr, "}\n");

    break;}//end of LET
  case NODE_LETRET:{
    struct nodeType *LHS = node->child;
    while (LHS->nodeType == NODE_PAIR) LHS = LHS->child;
    sqcodegen(fptr,LHS);
    assert(LHS->string);
    fprintf(fptr, "%s = %s;\n", node->string,LHS->string);

    break; }
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
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " < ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;

        /* remove pairs */
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;

        /*open new scope*/ 
        fprintf(fptr,"{\n");

        /*declare op1*/
        switch(op1->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
          break;
        default:
          assert(0);
        }

        /* declare op2*/
        switch(op2->valueType){
        case TypeInt:
          fprintf(fptr, "int op2;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op2;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op2;\n");
          break;
        default:
          assert(0);
        }

        /*assign op1*/
        switch(op1->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op1 = %d;\n", op1->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op1 = %f;\n", op1->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op1 = %s;\n", op1->string);
          break;
        default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;  
        }

        /*assign op2*/
        switch(op2->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op2 = %d;\n", op2->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op2 = %f;\n", op2->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op2 = %s;\n", op2->string);
          break;
        default:
          assert(op2->string);
          sqcodegen(fptr,op2);
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;  
        }

        fprintf(fptr, "%s = op1 < op2;\n",node->string);

        /*close scope*/
        fprintf(fptr,"}\n"); 
      }

      break;
    case OP_GT:
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " > ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;

        /* remove pairs */
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;

        /*open new scope*/ 
        fprintf(fptr,"{\n");

        /*declare op1*/
        switch(op1->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
          break;
        default:
          assert(0);
        }

        /* declare op2*/
        switch(op2->valueType){
        case TypeInt:
          fprintf(fptr, "int op2;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op2;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op2;\n");
          break;
        default:
          assert(0);
        }

        /*assign op1*/
        switch(op1->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op1 = %d;\n", op1->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op1 = %f;\n", op1->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op1 = %s;\n", op1->string);
          break;
        default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;  
        }

        /*assign op2*/
        switch(op2->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op2 = %d;\n", op2->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op2 = %f;\n", op2->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op2 = %s;\n", op2->string);
          break;
        default:
          assert(op2->string);
          sqcodegen(fptr,op2);
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;  
        }

        fprintf(fptr, "%s = op1 > op2;\n",node->string);

        /*close scope*/
        fprintf(fptr,"}\n"); 
      }

      break;

    case OP_LE:
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " <= ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;

        /* remove pairs */
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;

        /*open new scope*/ 
        fprintf(fptr,"{\n");

        /*declare op1*/
        switch(op1->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
          break;
        default:
          assert(0);
        }

        /* declare op2*/
        switch(op2->valueType){
        case TypeInt:
          fprintf(fptr, "int op2;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op2;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op2;\n");
          break;
        default:
          assert(0);
        }

        /*assign op1*/
        switch(op1->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op1 = %d;\n", op1->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op1 = %f;\n", op1->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op1 = %s;\n", op1->string);
          break;
        default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;  
        }

        /*assign op2*/
        switch(op2->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op2 = %d;\n", op2->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op2 = %f;\n", op2->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op2 = %s;\n", op2->string);
          break;
        default:
          assert(op2->string);
          sqcodegen(fptr,op2);
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;  
        }

        fprintf(fptr, "%s = op1 <= op2;\n",node->string);

        /*close scope*/
        fprintf(fptr,"}\n"); 
      }

      break;
    case OP_GE:
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " >= ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;

        /* remove pairs */
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;

        /*open new scope*/ 
        fprintf(fptr,"{\n");

        /*declare op1*/
        switch(op1->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
          break;
        default:
          assert(0);
        }

        /* declare op2*/
        switch(op2->valueType){
        case TypeInt:
          fprintf(fptr, "int op2;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op2;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op2;\n");
          break;
        default:
          assert(0);
        }

        /*assign op1*/
        switch(op1->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op1 = %d;\n", op1->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op1 = %f;\n", op1->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op1 = %s;\n", op1->string);
          break;
        default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;  
        }

        /*assign op2*/
        switch(op2->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op2 = %d;\n", op2->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op2 = %f;\n", op2->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op2 = %s;\n", op2->string);
          break;
        default:
          assert(op2->string);
          sqcodegen(fptr,op2);
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;  
        }

        fprintf(fptr, "%s = op1 >= op2;\n",node->string);

        /*close scope*/
        fprintf(fptr,"}\n"); 
      }

      break;
    case OP_EQ:
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " == ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;

        /* remove pairs */
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;

        /*open new scope*/ 
        fprintf(fptr,"{\n");

        /*declare op1*/
        switch(op1->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
          break;
        default:
          assert(0);
        }

        /* declare op2*/
        switch(op2->valueType){
        case TypeInt:
          fprintf(fptr, "int op2;\n");
          break;
        case TypeChar:
          fprintf(fptr, "char op2;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op2;\n");
          break;
        default:
          assert(0);
        }

        /*assign op1*/
        switch(op1->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op1 = %d;\n", op1->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op1 = %f;\n", op1->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op1 = %s;\n", op1->string);
          break;
        default:
          assert(op1->string);
          sqcodegen(fptr, op1);
          fprintf(fptr, "op1 = %s;\n",op1->string);
          break;  
        }

        /*assign op2*/
        switch(op2->nodeType){
        case NODE_TOKEN:
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;
        case NODE_INT:
          fprintf(fptr, "op2 = %d;\n", op2->iValue);
          break;
        case NODE_FLOAT:
          fprintf(fptr, "op2 = %f;\n", op2->rValue);
          break;
        case NODE_CHAR:
          fprintf(fptr, "op2 = %s;\n", op2->string);
          break;
        default:
          assert(op2->string);
          sqcodegen(fptr,op2);
          fprintf(fptr, "op2 = %s;\n",op2->string);
          break;  
        }

        fprintf(fptr, "%s = op1 == op2;\n",node->string);

        /*close scope*/
        fprintf(fptr,"}\n"); 
      }

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
    case OP_SHARP:{
      struct nodeType *op1 = node->child;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      fprintf(fptr,"{\n");
      assert(op1->valueType>=TypeSEQ_I); 
      switch(op1->nodeType){
      case NODE_TOKEN:
        assert(op1->string);
        fprintf(fptr, "%s=%s.len;\n",node->string,op1->string);
        break;
      default:
        assert(op1->string);
        sqcodegen(fptr, op1);
        fprintf(fptr, "%s=%s.len;\n",node->string,op1->string);
        break;
      }

      fprintf(fptr,"}\n");
      if(node->parent->nodeType == NODE_NESL)
        fprintf(fptr, "print_I(%s);\n", node->string);
      break;
    }
    case OP_BIND:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;

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
        sqcodegen (fptr,RHS);
        break;
      case NODE_TUPLE:
        assert(0); // not likely happened.
        break;
      case NODE_TOKEN:
        break;
      case NODE_PAIR:{
        while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
        sqcodegen(fptr, RHS);
        RHS= LHS->rsibling;
        break;}
      default :
        sqcodegen(fptr, RHS);
        assert(RHS->string);
        break;
      }// end of RHS->nodeType;

      switch(LHS->nodeType){
      case NODE_TOKEN:
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
        sqcodegen(fptr, LHS);
        break;
      case NODE_PATTERN:{
        //FIXME pattern in document is complicated.
        int i=0;
        while(!(LHS->string)&&i<=10) {LHS = LHS->child;i++;}
        fprintf(fptr, "%s",LHS->string);
        LHS= node->child;
        break;}
      case NODE_PAIR:{
        while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
        fprintf(fptr, "%s",LHS->string);
        LHS= LHS->rsibling;
        break;}
      default:
        fprintf(fptr, "%s",LHS->string);
        break;
      }
      LHS = node->child;

      fprintf(fptr, " = ");

      switch (RHS->nodeType){
      case NODE_TOKEN:
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
        sqcodegen(fptr, RHS);
        break;
      default:
        while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
        assert(RHS->string);
        fprintf(fptr, "%s",RHS->string);
        RHS= LHS->rsibling;
      }
      fprintf(fptr, ";\n");
      if(RHS->valueType >= TypeTuple_I){

      }
      while(LHS->nodeType == NODE_PATTERN) LHS=LHS->child;
      while(LHS->nodeType ==NODE_PAIR) LHS=LHS->child;
      if(LHS->nodeType == LHS_TUPLE||LHS->nodeType == NODE_TUPLE){
//        printEXPBINDTUPLE(fptr,LHS,RHS);
        //printBindTuple(fptr,LHS,RHS);
        sqcodegen(fptr, LHS);
      }
      fprintf(fptr,"//end of OP_BIND\n");
      break;
    }// end of OP_BIND;
    case OP_PP:
      // if child is a variable, it will be process here.
      if(node->child->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child->rsibling);

      fprintf(fptr, "CONCAT(%s, %s, %s, ",
              node->child->string,
              node->child->rsibling->string,
              node->string);
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
      }
      break; // end of OP_PP
    }// end of switch(node->op)
    break;
  }// end of NODE_OP

  case NODE_SEQ_REF:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    
    // generate the RHS's value if needed.
    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      break;
    case NODE_PAIR:
      while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
      sqcodegen(fptr, RHS);
      RHS= LHS->rsibling;
      break;
    default:
      sqcodegen(fptr, RHS);
      assert(RHS->string);
      break;
    }
    
    if(RHS->nodeType == NODE_INT){ // if RHS is simply a number
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,%d",node->string, LHS->string,RHS->iValue);
        fprintf(fptr, ");\n");
        break;
      case TypeSEQ_I:
        fprintf(fptr, "GET_ELEM_SEQ_I(%s,%s,%d",node->string, LHS->string,RHS->iValue);
        fprintf(fptr, ");\n");
        break;
      default:
        assert(0);
      }
    }else{  // if RHS is not simply a number.
      //sqcodegen(fptr, RHS); // print index.
      switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,%s",node->string, LHS->string,RHS->string);
        fprintf(fptr, ");\n");
        break;
      case TypeSEQ_I:
        fprintf(fptr, "GET_ELEM_SEQ_I(%s,%s,%s",node->string, LHS->string,RHS->string);
        fprintf(fptr, ");\n");
        break;
      case TypeTuple_F:
        fprintf(fptr, "GET_ELEM_PAIR_F(%s,%s,%s",node->string, LHS->string, RHS->string);
        fprintf(fptr, ");\n");
      break;
      default:
        assert(0);
      }
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
  case PARAM_TUPLE:{
    struct nodeType *LHS=node->child;
    struct nodeType *RHS = node->child->rsibling;
    struct nodeType *child = node->child;

    if(!node->isvisited){ // first time
      do{
        switch(child->nodeType){
        case NODE_TOKEN:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
          break;
        case NODE_PAIR:{
          struct nodeType *tmp = child;
          while(tmp->nodeType == NODE_PAIR) tmp = tmp->child;
          sqcodegen(fptr, tmp);
          assert(tmp->string);
          child->string = malloc(sizeof(char)*100);
          strcpy(child->string, tmp->string);
          //free(tmp->string);
          child->valueType = tmp->valueType;
          child->typeNode = tmp->typeNode;
          break;}
        case PARAM_TUPLE:
          sqcodegen(fptr,child);
          break;
        default:
          sqcodegen(fptr,child); 
          if(child->nodeType!=NODE_PAIR)
            assert(child->string);
          break;
        }
        child=child->rsibling;
      }while(child!=node->child);
      node->isvisited=1;
    }else{ // second time 
      while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
      switch(LHS->nodeType){
      case NODE_TOKEN:
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
        sqcodegen(fptr,LHS);
        break;
      default:
        fprintf(fptr,"%s",LHS->string);
        break;
      }
      LHS= node->child;
      fprintf(fptr, ", ");
      while(RHS->nodeType == NODE_PAIR) RHS= RHS->child;
      switch(RHS->nodeType){
      case NODE_TOKEN:
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case PARAM_TUPLE:
        sqcodegen(fptr,RHS);
        break;
      default:
        fprintf(fptr,"%s",RHS->string);
        break;
      }
      RHS= node->child->rsibling;
    }// end of if else

    break;}
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
  
  case NODE_SEQ:{
    struct nodeType *LHS = node->child;
    int deccount =0; 

    switch(LHS->nodeType){
    case NODE_INT:
    case NODE_BOOL:
    case NODE_CHAR:
    case NODE_FLOAT:
    case NODE_TOKEN:
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS=LHS->child;
      sqcodegen(fptr,LHS);
      assert(LHS->string);
      LHS = node->child;
      break;
    }

    fprintf(fptr, "MALLOC(%s, 1, struct Sequence);\n",node->string);

    switch(node->valueType){
    case TypeSEQ_F:
      fprintf(fptr, "SET_ELEM_F(");
      break;
    case TypeSEQ_I:
      fprintf(fptr, "SET_ELEM_I(");
      break;
    case TypeSEQ:
      switch(node->typeNode->valueType){
      case TypeTuple_F:
        fprintf(fptr, "SET_ELEM_PAIR_F(");
      break;
      default: 
      assert(0);
      break;
      }
      break;
    default:
      assert(0);
    }
    switch(LHS->nodeType){
    case NODE_INT:
    case NODE_BOOL:
    case NODE_CHAR:
    case NODE_FLOAT:
    case NODE_TOKEN:
      sqcodegen(fptr, LHS);
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS=LHS->child;
      fprintf(fptr, "%s",LHS->string);
      LHS = node->child;
      break;
    }
    fprintf(fptr, ",%s,0);\n",node->string);
    break;
  }// end of NODE_SEQ

  case NODE_NEW_SEQ:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    int deccount =0; 
    fprintf(fptr, "MALLOC(%s, %d, struct Sequence);\n",node->string, node->counts);

    switch(LHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", LHS->iValue, node->string, LHS->paramcount);
      break;
    case NODE_TOKEN:
      switch(LHS->valueType){
      case TypeSEQ_I:
        fprintf(fptr,"SET_ELEM_SEQ_I(%s,%s,%d);\n", 
                LHS->string, node->string, LHS->paramcount);
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
      break;
      break;
    case NODE_SEQ_TUPLE:
      sqcodegen(fptr, RHS);
      break;
    }

    if(LHS->valueType>=TypeSEQ_I)
      printAddREF(fptr, node->string, node->valueType, node);
    break;
  }// end of NODE_NEW_SEQ


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
    // FIXME support no nested situation.
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    struct nodeType *tmp = LHS;
    if(!node->isvisited){
      do{
        tmp = LHS;
        while(LHS->nodeType ==NODE_PAIR) LHS=LHS->child;
        switch(LHS->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
          break;
        default:
          sqcodegen(fptr, LHS);
          break;
        }
        LHS=tmp;
        LHS= LHS->rsibling;
      }while(LHS!=node->child);
      node->isvisited =1;
    }else{
      while(LHS->nodeType ==NODE_PAIR) LHS=LHS->child;
      switch(LHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
        break;
      default:
        sqcodegen(fptr, LHS);
        break;
      }
      LHS=tmp;
      LHS= LHS->rsibling;
      fprintf(fptr,", "); 
      while(RHS->nodeType ==NODE_PAIR) RHS=RHS->child;
      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
        break;
      default:
        sqcodegen(fptr, RHS);
        break;
      }
      RHS=tmp;
      RHS= RHS->rsibling;
      
    }
    //assert(0); //not implement;
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
    dumpTable(fptr, node);

    /*generate the src arrays.*/
    refaddcount = refTable.size;
    //sqcodegen(fptr, SRCARR);
    do{
      arrchild = rbchild->child->rsibling;
      if(arrchild->nodeType!=NODE_TOKEN)
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
    dumpTable(fptr, node->child); 
    /* get elem from src array.*/
    do{
      varchild = rbchild->child;
      arrchild = rbchild->child->rsibling;
      switch(varchild->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I");
        break;
      case TypeFloat:
        fprintf(fptr, "GET_ELEM_F");
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

    //fprintf(fptr, "%s = ", LHS->string);
    sqcodegen(fptr, LHS);

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
    case TypeTuple_F:
      fprintf(fptr, "SET_ELEM_PAIR_F");
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
    //if(rbchild->rsibling != rbchild){}
    fprintf(fptr, "FILTER_%d(%s, %s,",RBINDS->counts,node->string, FREVAR->string);
    
    switch(node->valueType){
    case TypeSEQ_I:
      fprintf(fptr, " int, I,\n");
      break;
    case TypeSEQ_F:
      fprintf(fptr, " float, F,\n");
      break;
    case TypeSEQ:
      switch(node->typeNode->valueType){
      case TypeTuple_F:
        fprintf(fptr, " struct Pair_F, PAIR_F,\n");
        break; 
      default:
        assert(0);//not implement;
      }
      break;
    default: 
      assert(0); // not implement;
    }
    
    do{
      fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
      switch(FREVAR->valueType){
      case TypeSEQ_I:
        fprintf(fptr, " int, I,\n");
        break;
      case TypeSEQ_F:
        fprintf(fptr, " float, F,\n");
        break;
      case TypeSEQ:
        switch(FREVAR->typeNode->valueType){
        case TypeTuple_F:
          fprintf(fptr, " struct Pair_F, PAIR_F,\n");
          break; 
        default:
          assert(0);//not implement;
        }
        break;
      case TypeInt:
        fprintf(fptr, " int, I,\n");
      break;
      case TypeFloat:
        fprintf(fptr, " float, F,\n");
      break;
      case TypeTuple_F:
        fprintf(fptr, " struct Pair_F, PAIR_F,\n");
      break;
      default: 
        assert(0); // not implement;
      }
      rbchild = rbchild->rsibling;
      FREVAR = rbchild->child;
      SRCARR = FREVAR->rsibling;
    }while(rbchild!=RBINDS->child);

    fprintf(fptr, "(");
    sqcodegen(fptr,FILTER->child);
    fprintf(fptr, "));\n");
    
    node->typeNode = FREVAR;
    assert(node->typeNode); 
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
  
  case LHS_TUPLE:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;
    
    assert(node->string);  
    
    switch (LHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      sqcodegen(fptr, LHS);
      fprintf(fptr, "=");
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      fprintf(fptr,"%s=",LHS->string);
      break;
    }
    if(node->valueType==TypeTuple){
      if(LHS->valueType<=TypeBool) fprintf(fptr, "(");
      else  fprintf(fptr, "*(");
      printtype(fptr, LHS->valueType);
      if(LHS->valueType<=TypeBool) fprintf(fptr, ")");
      else  fprintf(fptr, "*)");
    }
    fprintf(fptr,"%s.a;\n",node->string);

    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      sqcodegen(fptr, RHS);
      fprintf(fptr, "=");
      break;
    default:
      while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
      fprintf(fptr,"%s=",RHS->string);
      break;
    }
    if(node->valueType==TypeTuple){
      if(RHS->valueType<=TypeBool) fprintf(fptr, "(");
      else  fprintf(fptr, "*(");
      printtype(fptr, RHS->valueType);
      if(RHS->valueType<=TypeBool) fprintf(fptr, ")");
      else  fprintf(fptr, "*)");
    }
    fprintf(fptr,"%s.b;\n",node->string);
    switch (LHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      sqcodegen(fptr, LHS);
      LHS= node->child;
      break;
    }
    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      break;
    default:
      while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
      sqcodegen(fptr, RHS);
      assert(RHS->string);
      RHS= LHS->rsibling;
      break;
    }


    break;
  }
  case RHS_TUPLE:
  case NODE_TUPLE:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;
    switch (LHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      sqcodegen(fptr, LHS);
      LHS= node->child;
      break;
    }
    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      break;
    default:
      while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
      sqcodegen(fptr, RHS);
      assert(RHS->string);
      RHS= LHS->rsibling;
      break;
    }
    assert(node->string);
    fprintf(fptr,"%s.a = ",node->string);
    if(LHS->valueType >=TypeTuple_I)
      fprintf(fptr, "&");
    switch (LHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
      if(node->valueType == TypeTuple){
        fprintf(fptr, "(void*)");
      }
    case NODE_TOKEN:
      sqcodegen(fptr, LHS);
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      fprintf(fptr,"%s",LHS->string);
      break;
    }

    fprintf(fptr,";\n%s.b = ",node->string);
    if(RHS->valueType >=TypeTuple_I)
      fprintf(fptr, "&");
    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
      if(node->valueType == TypeTuple){
        fprintf(fptr, "(void*)");
      }
    case NODE_TOKEN:
      sqcodegen(fptr, RHS);
      break;
    case NODE_PAIR:
      while(!RHS->string) RHS=RHS->child;
      fprintf(fptr,"%s",RHS->string);
      RHS = LHS->rsibling;
      break;
    default:
      fprintf(fptr,"%s",RHS->string);
      break;
    }
    fprintf(fptr, ";\n"); 
    break;
  } // end of NODE_TUPLE

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
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;

    if(strcmp(node->child->string, "isContiguousList")==0){
      struct nodeType *param1,*param2,*param3,*tuple1;
      while(RHS->nodeType==NODE_PAIR) RHS=RHS->child;
      assert(RHS->nodeType == PARAM_TUPLE);
      param1 = RHS->child;
      assert(param1->rsibling->nodeType == PARAM_TUPLE);
      tuple1 =param1->rsibling;
      param2 = tuple1->child;
      param3 = param2->rsibling;
      for(int i =0;i<3;i++){
        if (i==0) param1 = param1;
        if (i==1) param1 = param2;
        if (i==2) param1 = param3;
        switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          break;
        default:
          sqcodegen(fptr,param1);
        }
      } 
      param1 = RHS->child;
      fprintf(fptr, "%s = isContiguousList(",node->string);
      for(int i =0;i<3;i++){
        if (i==0) param1 = param1;
        if (i==1) param1 = param2;
        if (i==2) param1 = param3;
        switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          sqcodegen(fptr,param1);
          break;
        default:
          fprintf(fptr,"%s",param1->string);
        }
        if(i<2) fprintf(fptr,",");
      }
      fprintf(fptr,");\n");

    }else if(strcmp(node->child->string, "genShuffledList")==0){
      struct nodeType *param1,*param2,*param3,*tuple1;

      while(RHS->nodeType==NODE_PAIR) RHS=RHS->child;
      assert(RHS->nodeType == PARAM_TUPLE);
      param1 = RHS->child;
      param2 = RHS->child->rsibling;
      
      for(int i =0;i<2;i++){
        if (i==0) param1 = param1;
        if (i==1) param1 = param2;
        if (i==2) param1 = param3;
        switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          break;
        default:
          sqcodegen(fptr,param1);
        }
      } 
      param1 = RHS->child;
      fprintf(fptr, "%s = genShuffledList(",node->string);
      for(int i =0;i<2;i++){
        if (i==0) param1 = param1;
        if (i==1) param1 = param2;
        if (i==2) param1 = param3;
        switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          sqcodegen(fptr,param1);
          break;
        default:
          fprintf(fptr,"%s",param1->string);
        }
        if(i<1) fprintf(fptr,",");
      }
      fprintf(fptr,");\n");
    }else if(strcmp(node->child->string, "genReverseList")==0){
      struct nodeType* param1 = RHS->child;
      switch(param1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        break;
      default:
        while(param1->nodeType == NODE_PAIR) param1=param1->child;
        sqcodegen(fptr,param1);
        param1 = RHS->child->child;
      }
      fprintf(fptr,"%s = genReverseList(", node->string);
      switch(param1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        sqcodegen(fptr,param1);
        break;
      default:
        while(param1->nodeType == NODE_PAIR) param1=param1->child;
        fprintf(fptr,"%s", param1->string);
        param1 = RHS->child->child;
      }
      fprintf(fptr, ");\n");

    }else if(strcmp(node->child->string, "dist")==0){
      struct nodeType* param1 = RHS->child->child;
      struct nodeType* param2 = RHS->child->child->rsibling;

      //first time it will prepare the parameters
      switch(param1->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        break;
      default:
        while(param1->nodeType == NODE_PAIR) param1=param1->child;
        sqcodegen(fptr,param1);
        param1 = param2->rsibling;
      }
      switch(param2->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        break;
      default:
        while(param2->nodeType == NODE_PAIR) param2=param2->child;
        sqcodegen(fptr,param2);
        param2 = param1->rsibling;
      }

      fprintf(fptr, "NESLDIST(%s,",node->string);
      switch(param1->nodeType){
      case NODE_TOKEN:
        fprintf(fptr,"%s,",param1->string);
        break;
      case NODE_INT:
        fprintf(fptr,"%d,",param1->iValue);
        break;
      case NODE_FLOAT:
        fprintf(fptr, "%f,",param1->rValue);
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
      //end of "rand" 
    }else if(strcmp(node->child->string, "time")==0){
      fprintf(fptr, "{\nDECT1T2\nfloat diff;\n");
      printtype(fptr, RHS->valueType);
      fprintf(fptr, " %s;\n", RHS->child->string);
      fprintf(fptr, "_t1 = CLOCK();\n");
      sqcodegen(fptr, RHS->child);
      //FIXME might be single number or a function name without parameter.
      // too many possibilies.
      fprintf(fptr, "_t2 = CLOCK();\n");

      fprintf(fptr, "diff = ((float)(_t2 - _t1) / CLOCKSPEED);\n");
      fprintf(fptr, "tm = diff;\n");
      int count =0;
      while(!RHS->string && count<10) {RHS=RHS->child; count++;}
      fprintf(fptr, "%s.a = %s;\n", node->string, RHS->string);
      RHS=LHS->rsibling;
      fprintf(fptr, "%s.b = tm;\n", node->string);
      fprintf(fptr, "}//end of function time(...)\n");

    }else if(strcmp(node->child->string, "rand")==0){
      struct nodeType *RHS = node->child->rsibling;
      while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        break;
      default:
        sqcodegen(fptr,RHS);
      }
      RHS = node->child->rsibling;

      fprintf(fptr,"%s = ",node->string);
      if(node->valueType ==TypeInt)
        fprintf(fptr, "RAND_I("); 
      else 
        fprintf(fptr, "RAND_F(");

      while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        sqcodegen(fptr, RHS);
        break;
      default:{
        int count =0;
        while(!RHS->string && count<10) {RHS=RHS->child; count++;}
        fprintf(fptr,"%s",RHS->string);
        RHS= node->child->rsibling;
        break;
      }
      }
      fprintf(fptr,");\n");

    }else if(strcmp(node->child->string, "plusp") ==0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType == NODE_PAIR) param = param->child;
      assert(!param->isvisited);
      switch(param->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
      break;
      default:
        sqcodegen(fptr, param);
      break;
      }
      fprintf(fptr, "plusp(");
      switch(param->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
      sqcodegen(fptr, param);
      break;
      default:
      fprintf(fptr, "%s", param->string);
      break;
      }
      fprintf(fptr, ")");
      if(node->parent->nodeType!=NODE_FILTER) fprintf(fptr,";\n");
    }else if(strcmp(node->child->string, "flatten") == 0){
    }else if(strcmp(LHS->string, "max_index") == 0){
    }else if(strcmp(LHS->string, "min_index") == 0){
    }else if(strcmp(LHS->string, "float") == 0){
    }else if(strcmp(LHS->string, "round") == 0){
    }else if(strcmp(LHS->string, "print_string") == 0){
    }else if(strcmp(LHS->string, "sum") == 0){
    }else {
      // user define function.
      while(RHS->nodeType == NODE_PAIR) RHS= RHS->child;
      //first time it will prepare the parameters
      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        break;
      default:
        while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
        sqcodegen(fptr,RHS);
        RHS = LHS->rsibling;
      }
      fprintf(fptr, "%s = %s",node->string, LHS->string);
      fprintf(fptr,"(");

      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
      case NODE_TOKEN:
        sqcodegen(fptr,RHS);
        break;
      default:
        while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
        if(RHS->nodeType == PARAM_TUPLE)
          sqcodegen(fptr, RHS); //second time it will print all the parameter names.
        else
          fprintf(fptr, "%s", RHS->string);
        RHS = LHS->rsibling;
      }
      fprintf(fptr, ");\n");
    }

    if(node->parent->nodeType == NODE_NESL){
      switch(node->valueType){
      case TypeTuple_SF:
        fprintf(fptr, "print_Tuple(%s, SEQ_I, F);\n", node->string);
        break;
      case TypeTuple_IF:
        fprintf(fptr, "print_Tuple(%s, I, F);\n", node->string);
        break;
      case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
      case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
      case TypeSEQ_I:
        fprintf(fptr, "print_SEQ_I(%s);\n", node->string);
        break;
      default:
        assert(0);
        break;
      }
    } 
    break;
  }// end of NODE_FUNC_CALL

  case NODE_TOKEN:
    switch(node->tokenType){
    case TOKE_ID:{
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


