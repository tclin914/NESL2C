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
    break;
  }

  case NODE_FUNC:{
    struct SymTableEntry * entry = findSymbol(node->child->table, node->string, REFERENCE);
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
      fprintf(fptr, "(");
      sqcodegen(fptr, parameter);
      fprintf(fptr, ")");
      fprintf(fptr, "{\nint _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr, parameter);
      sqcodegen(fptr,funcbody);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeFloat:
      fprintf(fptr, "float %s", node->string);
      fprintf(fptr, "(");
      sqcodegen(fptr, parameter);
      fprintf(fptr, ")");
      fprintf(fptr, "{\nfloat _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr, parameter);
      sqcodegen(fptr,funcbody);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeSEQ:
      fprintf(fptr, "struct Sequence  %s", node->string);
      fprintf(fptr, "(");
      sqcodegen(fptr, parameter);
      fprintf(fptr, ")");
      fprintf(fptr, "{\nstruct Sequence _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr, parameter);
      sqcodegen(fptr, funcbody);
      fprintf(fptr, "_res = %s;\n",funcbody->string);
      fprintf(fptr, "return _res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct ");
      gentypes(fptr, entry->link->typeNode);
      fprintf(fptr, " %s", node->string);
      fprintf(fptr, "(");
      sqcodegen(fptr, parameter);
      fprintf(fptr, ")");
      fprintf(fptr, "{\nstruct "); 
      gentypes(fptr, entry->link); 
      fprintf(fptr, " _res;\n");
      dumpTable(fptr, parameter);
      sqcodegen(fptr, parameter);
      sqcodegen(fptr, funcbody);
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
  case FPARAM_TOKEN:{
    if(!node->isvisited){
      struct SymTableEntry * entry;
      printtype(fptr, node); 
      assert(node->string);
      fprintf(fptr, " %s", node->string);
      entry = findSymbol(node->table, node->string, REFERENCE);
      assert(entry);
      entry ->isParam = 1;
      node->isvisited = 1;
    }else{
      ;//assert(0); 
    }
  break;
  }
  case FPARAM_TUPLE:{
    struct nodeType *LHS=node->child;
    struct nodeType *RHS=LHS->rsibling;
    if(!node->isvisited){
      struct SymTableEntry * entry;
      /* generate function prototype */
      printtype(fptr, node); 
      assert(node->string);
      fprintf(fptr, " %s", node->string);
      //entry = findSymbol(rootTable, node->string, REFERENCE);
      //assert(entry);
      //entry ->isParam = 1;
      node->isvisited = 1;
    }
    else{
      /* bind the original var with tuple node.
      ** linked by node->functioncall in use node->tuplenode as reference.
      ** tuplenode is linked with the original variables in codegencheck pass.
      */
      
      /* if child is token then we assign it with member of tuple. */
      switch(LHS->nodeType){
      case FPARAM_TUPLE:
        fprintf(fptr, "%s = %s.a;\n",LHS->string, node->string);
        LHS->isvisited=1;
        sqcodegen(fptr,LHS);
        break;
      case NODE_TOKEN:
        fprintf(fptr, "%s = %s.a;\n",LHS->string, node->string);
        break;
      default:
        assert(0);
        break;
      }
      switch(RHS->nodeType){
      case FPARAM_TUPLE:
        fprintf(fptr, "%s = %s.b;\n",RHS->string, node->string);
        RHS->isvisited=1;
        sqcodegen(fptr,RHS);
        break;
      case NODE_TOKEN:
        fprintf(fptr, "%s = %s.b;\n",RHS->string, node->string);
        break;
      default:
        assert(0);
        break;
      } 
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
    ////DECREF(fptr,refTable.size);
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
    break;}

  case NODE_LET:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
//    int refaddcount=refTable.size;
    fprintf(fptr, "{\n");
    
    dumpTable(fptr, node);
    sqcodegen(fptr, LHS);
    sqcodegen(fptr, RHS);
    assert(RHS->string);
//    assert(refTable.size>=refaddcount);
//    refaddcount = refTable.size-refaddcount;
//#ifdef DEBUG
//    fprintf(fptr,"//DEBUG LETrefaddcount:%d\n",refaddcount);
//#endif
    //DECREF(fptr,refaddcount);
    fprintf(fptr, "//end of LET\n");

    break;}//end of LET
  case NODE_LETRET:{
    struct nodeType *LHS = node->child;
    struct RefTableEntry *entry;
    int index;
//    int refaddcount=refTable.size;
    while (LHS->nodeType == NODE_PAIR) LHS = LHS->child;
    switch(LHS->nodeType){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_BOOL:
    case NODE_CHAR:
    case NODE_TOKEN:
    break;
    default:
    
    sqcodegen(fptr,LHS);
    break;
    }
    assert(LHS->string);
    fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
//    index = findREF(LHS->string);
//    if(index!=-1){
//      entry = &refTable.entries[index];
//      assert(LHS == entry->link);
//      deleteREF(index, index+1);
//    }
//    refaddcount = refTable.size - refaddcount;
//#ifdef DEBUG
//    fprintf(fptr,"//DEBUG LETRETrefaddcount:%d\n",refaddcount);
//#endif
    //DECREF(fptr,refaddcount);
    fprintf(fptr, "//end of LETRET\n");

    break; } // end of LETRET
  case NODE_BIND:{
    struct nodeType* child = node->child;
    if(child){
      do{
        sqcodegen(fptr, child);
        child = child->rsibling;
      }while(child!=node->child);
    }  
    break;
  } // end of NODE_BIND
  case NODE_EXP:
  case NODE_PATTERN:
    sqcodegen(fptr, node->child);
    break;
  case NODE_ASSIGN:{
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
      case NODE_INT:
        
        break;
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
      if(RHS->valueType == TypeTuple){

      }
      while(LHS->nodeType == NODE_PATTERN) LHS=LHS->child;
      while(LHS->nodeType ==NODE_PAIR) LHS=LHS->child;
      if(LHS->nodeType == LHS_TUPLE||LHS->nodeType == NODE_TUPLE){
        sqcodegen(fptr, LHS);
      }

      fprintf(fptr,"//end of ASSIGNMENT\n");
      break;
    }// end of NODE_ASSIGN;

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
    case OP_NE:
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " != ");
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

        fprintf(fptr, "%s = op1 != op2;\n",node->string);

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
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " + ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
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
      }
      break;
    }
    case OP_SUB:{
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " - ");
        sqcodegen(fptr, node->child->rsibling);
      }else{ 
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
      }
      break;
    }
    case OP_MUL:{
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " * ");
        sqcodegen(fptr, node->child->rsibling);
      }else{

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
      }
      break;
    }    
    case OP_DIV:{
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " / ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
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
      }
      break;
    } 
    case OP_UMINUS:{
      if(node->infilter){
        sqcodegen(fptr, node->child);
        fprintf(fptr, " - ");
        sqcodegen(fptr, node->child->rsibling);
      }else{
        struct nodeType *op1 = node->child;
        struct nodeType *op2 = op1->rsibling;
        while(op1->nodeType == NODE_PAIR) op1=op1->child;
        while(op2->nodeType == NODE_PAIR) op2=op2->child;
        fprintf(fptr,"{\n");
        printparam(fptr, node);// declare int add; etc...
        switch(node->valueType){
        case TypeInt:
          fprintf(fptr, "int op1;\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float op1;\n");
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
        
        fprintf(fptr, "%s = -op1;\n",node->string);
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
      }
      break;
    }
    case OP_UPT:{
      struct nodeType *op1 = node->child;
      struct nodeType *op2 = node->child->rsibling;
      assert(op1->valueType ==TypeFloat||op1->valueType ==TypeFloat);
      assert(op2->valueType ==TypeInt);
      if(node->infilter){
        fprintf(fptr, " pow( ");
        sqcodegen(fptr, node->child);
        fprintf(fptr, " , ");
        sqcodegen(fptr, node->child->rsibling);
        fprintf(fptr, " )");
      }else{
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
        fprintf(fptr, "%s=",node->string);
        if(op1->valueType == TypeInt) fprintf(fptr, "(int)");
        fprintf(fptr, " pow((float)op1, op2);\n");
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

      }
    break;
    }
    case OP_SHARP:{
      struct nodeType *op1 = node->child;
      while(op1->nodeType == NODE_PAIR) op1=op1->child;
      fprintf(fptr,"{\n");
      assert(op1->valueType==TypeSEQ); 
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
      //if(node->parent->nodeType == NODE_NESL)
      //  fprintf(fptr, "print_I(%s);\n", node->string);
      break;
    }
    case OP_PP:
      // if child is a variable, it will be process here.
      if(node->child->nodeType!=NODE_TOKEN&&node->child->nodeType!=NODE_SEQ)
        sqcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType!=NODE_TOKEN)
        sqcodegen(fptr, node->child->rsibling);
      
      if(node->child->nodeType == NODE_SEQ){
        fprintf(fptr, "PREPEND(%s, %s, %s, ",
              node->child->child->string,
              node->child->rsibling->string,
              node->string);
        
      }else{
      fprintf(fptr, "CONCAT(%s, %s, %s, ",
              node->child->string,
              node->child->rsibling->string,
              node->string);
      }
      switch(node->valueType){
      case TypeSEQ:
        switch(node->typeNode->child->valueType){
        case TypeTuple:
          printtype(fptr, node->typeNode->child);
          fprintf(fptr, ", ");
          gentypes(fptr, node->typeNode->child);
          fprintf(fptr, ");\n");
          //fprintf(fptr, "struct tuple, tuple);\n");
          break;
        case TypeInt:
          fprintf(fptr, "int , I);\n");
          break;
        case TypeFloat:
          fprintf(fptr, "float , F);\n");
          break;
        default:
          assert(0);
        }
        break;
      default:
        assert(0);//not implemented.
        break;
      }
      
      //FIXME problem in quicksort?  
      if(node->child->nodeType!=NODE_SEQ)
      ;//printAddREF(fptr, node->string, node->valueType, node);
      break; // end of OP_PP
    default:
      assert(0);// not implemented;
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
      case TypeFloat:
        fprintf(fptr, "GET_ELEM_F(%s,%s,%d",node->string, LHS->string,RHS->iValue);
        fprintf(fptr, ");\n");
        break;
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,%d",node->string, LHS->string,RHS->iValue);
        fprintf(fptr, ");\n");
        break;
      case TypeSEQ:
        //TODO
        fprintf(fptr, "GET_ELEM_SEQ");
        switch(node->typeNode->child->valueType){
        case TypeInt:
          fprintf(fptr, "_I");
          break;
        case TypeFloat:
          fprintf(fptr, "_F");
          break;
        default: 
          assert(0);
        } 
        fprintf(fptr, "(%s,%s,%d",node->string, LHS->string,RHS->iValue);
    
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
      case TypeSEQ:
        fprintf(fptr, "GET_ELEM_SEQ");
        switch(node->typeNode->child->valueType){
        case TypeInt:
          fprintf(fptr, "_I");
          break;
        case TypeFloat:
          fprintf(fptr, "_F");
          break;
        default: 
          assert(0);
        } 
        fprintf(fptr, "(%s,%s,%s",node->string, LHS->string,RHS->string);
        fprintf(fptr, ");\n");
        break;
      case TypeTuple:
        fprintf(fptr, "GET_ELEM_");
        gentypes(fptr, node);
        fprintf(fptr, "(%s,%s,%s",node->string, LHS->string, RHS->string);
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
      
    //}else{ 
      // second time 
      while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
      fprintf(fptr, "%s.a = ",node->string);
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
      fprintf(fptr, ";\n");
      fprintf(fptr, "%s.b = ",node->string);
      while(RHS->nodeType == NODE_PAIR) RHS= RHS->child;
      switch(RHS->nodeType){
      case NODE_TOKEN:
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_CHAR:
      case NODE_BOOL:
        sqcodegen(fptr,RHS);
        break;
      default:
        fprintf(fptr,"%s",RHS->string);
        break;
      }
      RHS= node->child->rsibling;
      fprintf(fptr, ";\n ");
    }// end of if else
    
    break;
    }//end of PARAM_TUPLE
  case NODE_SEQ_TUPLE:{
    struct nodeType *LHS=node->child;
    struct nodeType *RHS = node->child->rsibling;
    switch(LHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
              LHS->iValue, node->string, LHS->paramcount);
      break;
    case NODE_FLOAT:
      fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n",
              LHS->rValue, node->string, LHS->paramcount);
      break;
    default:
      sqcodegen(fptr, LHS);
      switch(LHS->valueType){
        case TypeInt:
          fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
              LHS->string, node->string, LHS->paramcount);
        break;
        case TypeFloat:
          fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
              LHS->string, node->string, LHS->paramcount);
        break;
        //case TypeTuple_I:
        //  fprintf(fptr, "SET_ELEM_PAIR_I(%s,%s,%d);\n",
        //      LHS->string, node->string, LHS->paramcount);
        //break;
        case TypeTuple:
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, LHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              LHS->string, node->string, LHS->paramcount);
        //  fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
        //      LHS->string, node->string, LHS->paramcount);
        break;
       default:
       assert(0);
       break;
        
      }
      break;
    //assert(0);
    }
    switch(RHS->nodeType){
    case NODE_SEQ_TUPLE:
      sqcodegen(fptr, RHS);
      break;
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
              RHS->iValue, node->string, RHS->paramcount);
      break;
    default:
      sqcodegen(fptr, RHS);
      switch(RHS->valueType){
        case TypeInt:
          fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
              RHS->string, node->string, RHS->paramcount);
        break;
        case TypeFloat:
          fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
              RHS->string, node->string, RHS->paramcount);
        break;
        case TypeTuple:
        //TODO
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, RHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              RHS->string, node->string, RHS->paramcount);
        break;
       default:
       assert(0);
        
      }    
    }
    break;
  }// end of NODE_SEQ_TUPLE
  
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

    fprintf(fptr, "MALLOC(%s,1,",node->string);

    switch(LHS->valueType){
    case TypeInt:
      fprintf(fptr, "int");
      break;
    case TypeFloat:
      fprintf(fptr, "float");
      break;
    case TypeBool:
      fprintf(fptr, "bool");
      break;
    case TypeChar:
      fprintf(fptr, "char");
      break;
    case TypeSEQ:
      fprintf(fptr, "struct Sequence");
      break;
    case TypeTuple:
      fprintf(fptr, "struct t_");
      assert(0);// not implement;
      break;
    default:
      assert(0);
      break;
    }
    fprintf(fptr,");\n");

    switch(node->valueType){
    case TypeSEQ:
      switch(node->typeNode->valueType){
      case TypeTuple:
        //TODO
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, node->typeNode);
        fprintf(fptr, "(");
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
    while(LHS->nodeType == NODE_PAIR) LHS=LHS->child;
    while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
    fprintf(fptr, "MALLOC(%s,%d,",node->string, node->counts);

    switch(LHS->valueType){
    case TypeInt:
      fprintf(fptr, "int");
      break;
    case TypeFloat:
      fprintf(fptr, "float");
      break;
    case TypeBool:
      fprintf(fptr, "bool");
      break;
    case TypeChar:
      fprintf(fptr, "char");
      break;
    case TypeSEQ:
      fprintf(fptr, "struct Sequence");
      break;
    case TypeTuple:
      printtype(fptr, LHS);
      //fprintf(fptr, "s`truct tuple");
      break;
    default:
      assert(0);
      break;
    }
    fprintf(fptr,");\n");


    switch(LHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", LHS->iValue, node->string, LHS->paramcount);
      break;
    case NODE_FLOAT:
      fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n", LHS->rValue, node->string, LHS->paramcount); 
      break;
    case NODE_TOKEN:
      switch(LHS->valueType){
      case TypeSEQ:
        fprintf(fptr,"SET_ELEM_SEQ"); 
        switch(LHS->typeNode->child->valueType){
        case TypeInt:
        fprintf(fptr, "_I");
        break;
        case TypeFloat:
        fprintf(fptr, "_F");
        break;
        default:
        assert(0);
        }
        fprintf(fptr,"(%s,%s,%d);\n", 
                LHS->string, node->string, LHS->paramcount);
        break;
      case TypeTuple:
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, LHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              LHS->string, node->string, LHS->paramcount);
        //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
        //        LHS->string, node->string, LHS->paramcount);
      break;
      default:
        assert(0);
        break;
      }
      break;
    default:
      sqcodegen(fptr, LHS);
      switch(LHS->valueType){
      case TypeInt:
        fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                LHS->string, node->string, LHS->paramcount);
      break;
      case TypeFloat:
        fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                LHS->string, node->string, LHS->paramcount);
      break;
      case TypeTuple:
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, LHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              LHS->string, node->string, LHS->paramcount);
        //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
        //        LHS->string, node->string, LHS->paramcount);
      break;
      default:
      assert(0);
      }
    break;
    }
    switch(RHS->nodeType){
    case NODE_INT:
      fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", RHS->iValue, node->string, RHS->paramcount); 
      break;
    case NODE_FLOAT:
      fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n", RHS->rValue, node->string, RHS->paramcount); 
      break;
    case NODE_TOKEN:
      switch(RHS->valueType){
      case TypeSEQ:
        fprintf(fptr,"SET_ELEM_SEQ"); 
        switch(LHS->typeNode->child->valueType){
        case TypeInt:
        fprintf(fptr, "_I");
        break;
        case TypeFloat:
        fprintf(fptr, "_F");
        break;
        default:
        assert(0);
        }
        fprintf(fptr,"(%s,%s,%d);\n", 
                RHS->string, node->string, RHS->paramcount);
        break;
      case TypeTuple:
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, RHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              RHS->string, node->string, RHS->paramcount);
        //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
        //        RHS->string, node->string, RHS->paramcount);
      break;
      default:
        assert(0);
        break;
      }
      break;
    case NODE_SEQ_TUPLE:
      sqcodegen(fptr, RHS);
      break;
    default:
      sqcodegen(fptr, RHS);
      switch(RHS->valueType){
      case TypeInt:
        fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                RHS->string, node->string, RHS->paramcount);
      break;
      case TypeFloat:
        fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                RHS->string, node->string, RHS->paramcount);
      break;
      case TypeTuple:
        fprintf(fptr, "SET_ELEM_");
        gentypes(fptr, RHS);
        fprintf(fptr, "(%s,%s,%d);\n",
              RHS->string, node->string, RHS->paramcount);
        //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
        //        RHS->string, node->string, RHS->paramcount);
      break;
      default:
      assert(0);
      }
    break;
    }

    break;
  }// end of NODE_NEW_SEQ

  case NODE_APPLYBODY1:
    abort();
    break;

  case NODE_ACTION_TUPLE:{
    // FIXME support no nested situation.
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    struct nodeType *tmp = LHS;
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
      
      fprintf(fptr, "%s.a = ",node->string);
      while(LHS->nodeType ==NODE_PAIR) LHS=LHS->child;
      switch(LHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
        sqcodegen(fptr, LHS);
        break;
      default:
        fprintf(fptr,"%s", LHS->string);
        break;
      }
      LHS=tmp;
      LHS= LHS->rsibling;
      fprintf(fptr, ";\n");
      
      fprintf(fptr, "%s.b = ", node->string);
      while(RHS->nodeType ==NODE_PAIR) RHS=RHS->child;
      switch(RHS->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
        sqcodegen(fptr, RHS);
        break;
      default:
        fprintf(fptr, "%s", RHS->string);
        break;
      }
      fprintf(fptr, ";\n");
      RHS=tmp;
      RHS= RHS->rsibling;

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
    fprintf(fptr, "MALLOC(%s,%s.len,",node->string, arrchild->string);
    
    switch(LHS->valueType){
    case TypeInt:
      fprintf(fptr, "int");
    break;
    case TypeFloat:
      fprintf(fptr, "float");
    break;
    case TypeBool:
      fprintf(fptr, "bool");
    break;
    case TypeChar:
      fprintf(fptr, "char");
    break;
    case TypeSEQ:
      fprintf(fptr, "struct Sequence");
    break;
    case TypeTuple:
      fprintf(fptr, "struct ");
      gentypes(fptr,LHS);
    break;
    default:
    assert(0);
    break;
    }
    fprintf(fptr,");\n");

    /* generate for loop */
    forlooprefaddcount = refTable.size;
    fprintf(fptr, "_len = %s.len;\n",node->string);
    fprintf(fptr, "#pragma pf parallel_rr\n");
    fprintf(fptr, "for (_i =0; _i <_len;_i++){\n");
    dumpTable(fptr, node->child); 
    /* get elem from src array.*/
    do{
      varchild = rbchild->child;
      arrchild = rbchild->child->rsibling;
      while(varchild->nodeType == NODE_PAIR) varchild = varchild->child;
      switch(varchild->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I");
        break;
      case TypeFloat:
        fprintf(fptr, "GET_ELEM_F");
        break;
      case TypeSEQ:
      //TODO
        fprintf(fptr, "GET_ELEM_SEQ");
        switch(varchild->typeNode->child->valueType){
        case TypeInt:
        fprintf(fptr, "_I");
        break;
        case TypeFloat :
        fprintf(fptr, "_F");
        break;
        default:
        assert(0); //TODO
        }
        break;
      case TypeTuple:
      //TODO
        fprintf(fptr, "GET_ELEM_");
        gentypes(fptr, varchild);
        break;
      default:
        assert(0);
        break;
      }
      assert(varchild->string);
      fprintf(fptr, "(%s,%s,_i);\n",varchild->string,arrchild->string);
      if(varchild->nodeType == RB_TUPLE){
        sqcodegen(fptr,varchild);
      }
      rbchild= rbchild->rsibling;
    }while(rbchild!=RBINDS->child);

    switch(LHS->nodeType){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_BOOL:
    case NODE_CHAR:
    case NODE_TOKEN:
    break;
    default:
    sqcodegen(fptr, LHS);
    }
    
    switch(LHS->valueType){
    case TypeInt:
      fprintf(fptr, "SET_ELEM_I");
      break;
    case TypeFloat:
      fprintf(fptr, "SET_ELEM_F");
      break;
    case TypeSEQ:{
      int x=0;
      struct nodeType *loopme = LHS->typeNode->child;
      fprintf(fptr, "SET_ELEM_SEQ");
      while(loopme->valueType == TypeSEQ){
        fprintf(fptr,"_SEQ");
        loopme = loopme->typeNode->child;
        if(x++==10) abort();
        }
      switch(loopme->valueType){
      case TypeInt:
        fprintf(fptr, "_I");
        break;
      case TypeFloat:
        fprintf(fptr, "_F");
        break;
      case TypeTuple:
        fprintf(fptr, "_");
        gentypes(fptr, loopme->typeNode);
        break;
      }
    break;}
    case TypeTuple:
      fprintf(fptr, "SET_ELEM_");
      gentypes(fptr, LHS->typeNode);
    //TODO
    //assert(0);
    break;
    default:
      assert(0);
      break;
    }
    fprintf(fptr, "(%s,%s,_i);\n", LHS->string, node->string);
    forlooprefaddcount = refTable.size - forlooprefaddcount;
#ifdef DEBUG
    fprintf(fptr,"//DEBUG forlooprefaddcount:%d\n",forlooprefaddcount);
#endif
    ////DECREF(fptr,forlooprefaddcount);
    if(containArray(LHS)) {
      fprintf(fptr, "\n//release(%s); %d\n", LHS->string, LHS->nodeType);
      printDECREF(fptr,LHS);
    }
    fprintf(fptr, "}\n");// close for
    /* release the arrchilds */
    do{
      arrchild = rbchild->child->rsibling;
      if(containArray(arrchild)){
        //sqcodegen(fptr, arrchild);
        if(arrchild->nodeType!=NODE_TOKEN){
          fprintf(fptr, "\n//release arrchild :%s , nodeType:%d\n", arrchild->string, arrchild->nodeType);
          printDECREF(fptr, arrchild);
        }
      }
      else assert(0);
      rbchild = rbchild->rsibling;
    }while(rbchild!=RBINDS->child);



#if DEBUG
    fprintf(fptr,"//DEBUG refaddcount:%d\n",refaddcount);
#endif
    //DECREF(fptr,refaddcount);
    //node->typeNode = node->child;
    //printAddREF(fptr, node->string, node->valueType, node);
    fprintf(fptr, "}\n");//close scope

    break;
  }
  case GEN_APP3:{
    struct nodeType *APP3 = node->child->rsibling;
    struct nodeType *RBINDS = APP3->child;
    struct nodeType *rbchild = RBINDS->child;
    struct nodeType *FREVAR = rbchild->child;
    struct nodeType *SRCARR = FREVAR->rsibling;
    struct nodeType *FILTER = RBINDS->rsibling; 

    //do{}while(rbchild!=RBINDS->child); //this is for future FILTER_2.
    //if(rbchild->rsibling != rbchild){}
    fprintf(fptr, "FILTER_%d(%s, %s, ",RBINDS->counts,node->string, FREVAR->string);
    
    switch(node->valueType){
    case TypeSEQ:
      switch(node->typeNode->child->valueType){
      case TypeTuple:
        printtype(fptr, node->typeNode->child->typeNode);
        fprintf(fptr, ", ");
        gentypes(fptr, node->typeNode->child->typeNode);
        fprintf(fptr, ",\n");
        break; 
      case TypeInt:
        fprintf(fptr, " int, I,\n");
      break;
      case TypeFloat:
        fprintf(fptr, " float, F,\n");
      break;
      case TypeBool:
      case TypeChar:
      default:
        assert(0);//not implement;
      }
      break;
    default: 
      assert(0); // not implement;
    }
    
    do{
      while(FREVAR->nodeType == NODE_PAIR) FREVAR = FREVAR->child;
      fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
      switch(FREVAR->valueType){
      case TypeSEQ:
        switch(FREVAR->typeNode->valueType){
        case TypeTuple:
          assert(0);
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
      case TypeTuple:
        printtype(fptr, node->typeNode->child->typeNode);
        fprintf(fptr, ", ");
        gentypes(fptr, node->typeNode->child->typeNode);
        fprintf(fptr, ",\n");
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
    
    //node->typeNode = FREVAR->typeNode;
    //assert(node->typeNode); 
    //printAddREF(fptr, node->string, node->valueType, node);

    break;
  } // end of GEN_APP3

  case NODE_APPLYBODY3:{
    struct nodeType *RBINDS = node->child;
    struct nodeType *rbchild = RBINDS->child;
    struct nodeType *FREVAR = rbchild->child;
    struct nodeType *SRCARR = FREVAR->rsibling;
    struct nodeType *FILTER = RBINDS->rsibling; // TODO remove the pair in yacc.
    fprintf(fptr, "{\n");
    //dumpTable(fptr, node);
    switch(FREVAR->nodeType){
      case RB_TUPLE:
        //sqcodegen(fptr,FREVAR);
        fprintf(fptr, "FILTER_TUPLE_%d(%s, %s,", 
                    RBINDS->counts,node->string, FREVAR->string);
        // FIXME dirtyway
        fprintf(fptr, "%s, %s,", FREVAR->child->string, FREVAR->child->rsibling->string);
        printtype(fptr, FREVAR->child);
        fprintf(fptr, ", ");
        printtype(fptr, FREVAR->child->rsibling);
        fprintf(fptr, ",\n");
        break;
      default:
        fprintf(fptr, "FILTER_%d(%s, %s, ",RBINDS->counts,node->string, FREVAR->string);
        break;
    }

    switch(node->valueType){
    case TypeSEQ:
      switch(node->typeNode->child->valueType){
      case TypeTuple:
        printtype(fptr, node->typeNode->child);
        fprintf(fptr, ", ");
        gentypes(fptr, node->typeNode->child);
        fprintf(fptr, ",\n ");
        break; 
      default:
        assert(0);//not implement;
      }
      break;
    default: 
      assert(0); // not implement;
    }

    do{
      while(FREVAR->nodeType == NODE_PAIR) FREVAR = FREVAR->child;
      fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
      switch(FREVAR->valueType){
      case TypeSEQ:
        switch(FREVAR->typeNode->child->valueType){
        case TypeTuple:
          assert(0);
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
      case TypeTuple:
        printtype(fptr, FREVAR->typeNode);
        fprintf(fptr, ", ");
        gentypes(fptr, FREVAR->typeNode);
        fprintf(fptr, ",\n");
        //assert(0);
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
    fprintf(fptr, "}\n"); 

    //node->typeNode = FREVAR;
    assert(node->typeNode); 
 
    //FIXME dirty4ni~~
    if(node->parent->nodeType == NODE_LETRET){
      printf("boom\n");
    }else{
    //printAddREF(fptr, node->string, node->valueType, node);
    }
    
    break;
  }

  case NODE_APPLYBODY4:
    assert(0);
    break;

  case NODE_RBINDS:
    //FIXME might have many children
    // has been done by the upper level apply-to-each.
    sqcodegen(fptr, node->child);
    break;

  case NODE_IN:
    //FIXME here only consider the simple ApplyBody Exp.
    //assert(node->child->nodeType == NODE_TOKEN);
    //assert(node->child->rsibling->nodeType == NODE_TOKEN);
    
    sqcodegen(fptr, node->child->rsibling);
    //    node->string
    break;
  
  case RB_TUPLE:
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
      fprintf(fptr, " = ");
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      fprintf(fptr,"%s = ",LHS->string);
      break;
    }
    fprintf(fptr,"%s.a;\n",node->string);

    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      sqcodegen(fptr, RHS);
      fprintf(fptr, " = ");
      break;
    default:
      while(RHS->nodeType == NODE_PAIR) RHS = RHS->child;
      fprintf(fptr,"%s = ",RHS->string);
      break;
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
  case ELEM_TUPLE:
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
    switch (LHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
    case NODE_TOKEN:
      sqcodegen(fptr, LHS);
      break;
    default:
      while(LHS->nodeType == NODE_PAIR) LHS = LHS->child;
      fprintf(fptr,"%s",LHS->string);
      break;
    }

    fprintf(fptr,";\n%s.b = ",node->string);
    switch (RHS->nodeType ){
    case NODE_INT:
    case NODE_FLOAT:
    case NODE_CHAR:
    case NODE_BOOL:
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
    }else if(strcmp(node->child->string, "verifyQHull")==0){
      while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
      struct nodeType *param1 = RHS->child;
      struct nodeType *param2 = param1->rsibling;
      assert(param1->valueType ==TypeInt);
      assert(param2->valueType ==TypeSEQ);
      switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
        break;
        default: 
          sqcodegen(fptr, param1);
          break;
      }
      switch(param2->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
        break;
        default: 
          sqcodegen(fptr, param2);
          break;
      }
      fprintf(fptr, "%s = verifyQHull(",node->string);
      switch(param1->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          sqcodegen(fptr, param1);
        break;
        default: 
          fprintf(fptr, "%s",param1->string);
          break;
      }
      fprintf(fptr, ",");
      switch(param2->nodeType){
        case NODE_INT:
        case NODE_TOKEN:
          sqcodegen(fptr, param2);
        break;
        default: 
          fprintf(fptr, "%s",param2->string);
          break;
      }
      fprintf(fptr, ");\n");
    }else if(strcmp(node->child->string, "genReverseList")==0){
      struct nodeType* param1 = RHS;
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
      struct nodeType* param1 = RHS->child;
      struct nodeType* param2 = RHS->child->rsibling;

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
      if(param1->valueType == TypeInt)
      fprintf(fptr, "NESLDIST(%s,",node->string);
      else if(param1->valueType == TypeFloat)
      fprintf(fptr, "NESLDIST_F(%s,",node->string);
      else assert(0); // not support.
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
    }else if(strcmp(node->child->string, "time")==0){
      fprintf(fptr, "{\nDECT1T2\nfloat diff;\n");
      //printtype(fptr, RHS->valueType);
      //fprintf(fptr, " %s;\n", RHS->child->string);
      fprintf(fptr, "_t1 = CLOCK();\n");
      sqcodegen(fptr, RHS);
      //FIXME might be single number or a function name without parameter.
      // too many possibilies.
      fprintf(fptr, "_t2 = CLOCK();\n");

      fprintf(fptr, "diff = ((float)(_t2 - _t1) / CLOCKSPEED);\n");
      //fprintf(fptr, "tm = diff;\n");
      int count =0;
      while(!RHS->string && count<10) {RHS=RHS->child; count++;}
      //FIXME dirty
      //if(RHS->valueType >=TypeSEQ_I&&RHS->valueType<=TypeSEQ)
      //  RHS->isEndofFunction = 1; 
      #ifdef DEBUG
      fprintf(fptr, "//DEBUG %s.isEndofFunction:%d\n",node->string, node->isEndofFunction);
      fprintf(fptr, "//DEBUG %s.isEndofFunction:%d\n",RHS->string, RHS->isEndofFunction);
      #endif
      fprintf(fptr, "%s.a = %s;\n", node->string, RHS->string);
      RHS=LHS->rsibling;
      fprintf(fptr, "%s.b = diff;\n", node->string);
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
      struct nodeType *param = node->child->rsibling;
      struct nodeType *typer = param->child->typeNode;
      while(param->nodeType==NODE_PAIR) param = param->child;
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
      
      fprintf(fptr, "FLATTEN(");
      
      switch(param->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        sqcodegen(fptr, param);
        break;
        default:
        fprintf(fptr, "%s",param->string);
        break;
      }
      fprintf(fptr, ",%s,",node->string);
      
      switch(typer->child->valueType){
        case TypeTuple:
        printtype(fptr, typer->child->typeNode);
        fprintf(fptr, ",");
        gentypes(fptr, typer->child->typeNode);
        break;
        default:
        assert(0);
        break;
      }
      fprintf(fptr, ");\n");
      break;
      //assert(0);
    }else if(strcmp(LHS->string, "max_index") == 0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType==NODE_PAIR) param = param->child;
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
      switch (param->valueType){
        case TypeSEQ: 
          fprintf(fptr, "%s = max_index_f(",node->string);
        break;
        default:
          assert(0);// not implement;
      }
      switch(param->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        sqcodegen(fptr, param);
        break;
        default:
        fprintf(fptr, "%s",param->string);
        break;
      }
      fprintf(fptr, ");\n");
      return ;
    }else if(strcmp(LHS->string, "min_index") == 0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType==NODE_PAIR) param = param->child;
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
      switch (param->valueType){
        case TypeSEQ: 
          fprintf(fptr, "%s = min_index_f(",node->string);
        break;
        default:
          assert(0);// not implement;
      }
      switch(param->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        sqcodegen(fptr, param);
        break;
        default:
        fprintf(fptr, "%s",param->string);
        break;
      }
      fprintf(fptr, ");\n");
      return ;
    }else if(strcmp(LHS->string, "float") == 0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType==NODE_PAIR) param = param->child;
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
      switch(param->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        fprintf(fptr, "%s = (float)",node->string);
        sqcodegen(fptr, param);
        fprintf(fptr, ";\n");
        break;
        default:
        fprintf(fptr, "%s = (float)%s;\n",node->string,param->string);
        break;
      } 
      return;
    }else if(strcmp(LHS->string, "round") == 0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType==NODE_PAIR) param = param->child;
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
      fprintf(fptr, "%s = nearbyint(",node->string);
      switch(param->nodeType){
      case NODE_INT:
      case NODE_FLOAT:
      case NODE_BOOL:
      case NODE_CHAR:
      case NODE_TOKEN:
        sqcodegen(fptr, param);
        break;
      default:
        fprintf(fptr, "%s",param->string);
        break;
      }    
      fprintf(fptr, ");\n");
    }else if(strcmp(LHS->string, "print_string") == 0){
      struct nodeType *param = node->child->rsibling;
      while(param->nodeType==NODE_PAIR) param = param->child;
      assert(param ->nodeType == NODE_STRING);
       
      fprintf(fptr, "printf(%s);\n", param->string);
      fprintf(fptr, "%s = 1;\n",node->string);
      return ;
    }else if(strcmp(LHS->string, "mod") == 0){
      struct nodeType *param1 = node->child->rsibling->child->child;
      struct nodeType *param2 = param1->rsibling;
      switch(param1->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        break;
        default:
          sqcodegen(fptr, param1);
      }
      switch(param2->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        break;
        default:
          sqcodegen(fptr, param2);
      }
      fprintf(fptr, "%s = ",node->string);
      switch(param1->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        sqcodegen(fptr, param1);
        break;
        default:
          fprintf(fptr, "%s",param1->string);
      }
      /* print '%' in ascii */
      fprintf(fptr, "%c",37);
      switch(param2->nodeType){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
        sqcodegen(fptr, param2);
        break;
        default:
          fprintf(fptr, "%s",param2->string);
      }
      fprintf(fptr, ";\n");
      break;
    }else if(strcmp(LHS->string, "sum") == 0){
    assert(0);
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
          //sqcodegen(fptr, RHS); //second time it will print all the parameter names.
          fprintf(fptr, "%s", RHS->string);
        else
          fprintf(fptr, "%s", RHS->string);
        RHS = LHS->rsibling;
      }
      fprintf(fptr, ");\n");
    }

    if(node->valueType==TypeSEQ)
      //printAddREF(fptr,node->string, node->valueType, node);
      ;
    break;
  }// end of NODE_FUNC_CALL
  case NODE_TOKEN:
    
    if(node->parent->nodeType == NODE_NESL){
      switch(node->valueType){
        struct nodeType *loopme; int x;
      case TypeTuple:
        fprintf(fptr, "print_Tuple(%s,", node->string);
        switch(node->typeNode->child->valueType){
        case TypeSEQ:
          loopme = node->typeNode->child; 
          x=0;
          while(loopme->valueType ==TypeSEQ){
            fprintf(fptr, "SEQ_");
            loopme = loopme->typeNode;
            assert(loopme);
            if(x++==10) abort();//error;
          }
          switch(loopme->valueType){
          case TypeSEQ:  
            assert(0);//not implement;
            break;
          case TypeFloat:
            fprintf(fptr, "F");
            break;
          case TypeTuple:
            //TODO 
            assert(0);
            break;
          default:
            assert(0); //not implement
          }
          fprintf(fptr, ", F);\n");
          break;
        default:
          assert(0);
        }
        break;
      case TypeInt:
        fprintf(fptr, "print_I(%s);\n", node->string);
        break;
      case TypeFloat:
        fprintf(fptr, "print_F(%s);\n", node->string);
        break;
      case TypeSEQ:
        //TODO fprintf(fptr, "print_SEQ_I(%s);\n", node->string);
        assert(0);
        break;
      default:
        assert(0);
        break;
      }
    }else{
      switch(node->tokenType){
      case TOKE_ID:{
        struct SymTableEntry* entry = findSymbol(node->table, node->string, REFERENCE);
        assert(entry);
        fprintf(fptr, "%s", node->string);
        break;
      }
      default:
      assert(0);
      break;
      }
    }
    break;
  default:
    break;
  }// switch node->nodeType
  
  if(node->parent){
    if(node->parent->nodeType == NODE_NESL 
      &&node->nodeType!=NODE_FUNC
      &&node->nodeType!=NODE_ASSIGN
//      &&node->nodeType!=NODE_APPLYBODY2
//      &&node->nodeType!=NODE_APPLYBODY3
      ){
      switch(node->valueType){
        struct nodeType *loopme; int x;
      case TypeInt:
        if(node->nodeType==NODE_INT) fprintf(fptr,"printf(%d\\n);\n",node->iValue);
        else fprintf(fptr, "print_I(%s);\n",node->string);
      break;
      case TypeChar:
        assert(0);
      break;
      case TypeFloat:
        if(node->nodeType==NODE_INT) fprintf(fptr,"printf(%f\\n);\n",node->rValue);
        else fprintf(fptr, "print_F(%s);\n",node->string);
      break;
      case TypeBool:
        assert(0);
      break;
      case TypeTuple:{
        struct nodeType *child = node->typeNode->child;
        fprintf(fptr, "print_Tuple(%s,", node->string);
        do{
          switch(child->valueType){
          case TypeSEQ:
            gentypes(fptr, child);
            //assert(0); // TODO use the gentype(); 
            break;
          case TypeFloat:
            fprintf(fptr, "F");
            break;
          case TypeInt:
            fprintf(fptr, "I");
            break;
          default:
            assert(0);
          }
          if(child==node->typeNode->child)
            fprintf(fptr,",");
          child = child->rsibling;
        }while(child!=node->typeNode->child);
        fprintf(fptr,");\n");
      break;
      }
      case TypeSEQ:{
        fprintf(fptr, "print_SEQ_");
        loopme = node->typeNode->child; 
        x=0;
        while(loopme->valueType ==TypeSEQ){
          fprintf(fptr, "SEQ_");
          loopme = loopme->typeNode->child;
          assert(loopme);
          if(x++==10) abort();//error;
        }
        switch(loopme->valueType){
        case TypeSEQ:  
          fprintf(fptr, "SEQ_");
          //assert(0);//not implement;
          break;
        case TypeInt:
          fprintf(fptr, "I");
          break;
        case TypeFloat:
          fprintf(fptr, "F");
          break;
        case TypeTuple:
          gentypes(fptr, loopme);
          //fprintf(fptr, "T_");
          break;
        default:
          assert(0); //not implement
        }
        fprintf(fptr, "(%s);\n", node->string);
        break;
      }
      default:
        assert(0);
        break;
      }
    }
  }
      
  /*
  * print addrefcnt 
  */
  if(containArray(node)){
    switch(node->nodeType){
    case NODE_APPLYBODY1:
    case NODE_APPLYBODY2:
    case NODE_APPLYBODY3:
    case NODE_APPLYBODY4:
    case GEN_APP3:
    case NODE_NEW_SEQ:
    case PARAM_TUPLE:
        printAddREF(fptr,node->string,node->valueType,node); 
    break;
    case NODE_OP:
      switch(node->op){
      case OP_PP:
        printAddREF(fptr,node->string,node->valueType,node); 
      break;
      default:
      break; 
      }
    break;
    case NODE_THENSTMT:
    case NODE_ELSESTMT:
        printAddREF(fptr,node->string,node->valueType,node->typeNode); 
    break;
    //case NODE_FUNC_CALL:
    //    switch(node->parent->nodeType){
    //    case NODE_LETRET:
    //    break;
    //    default:
    //    printAddREF(fptr,node->string,node->valueType,node); 
    //    break;
    //    }
    //break;
    default:
    break;
    }
  }
   
  /*
  * print DECREF 
  */
  child = node->child;
  if(child){
    do{
      if(containArray(child)){
        DecRefCountForContainedArray(fptr, child); 
      }
      child = child->rsibling;
    }while(child!=node->child);
  }
  
  switch(node->nodeType){
  case NODE_THENSTMT:
  case NODE_ELSESTMT:
  case NODE_LET:
    fprintf(fptr, "\n}\n");
  break;
  }

  return;// end of sqcodegen.
}


