#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"
#include "pfcodegen.h"

#define MAX 10

struct RefTable refTable;
int elmindex[MAX];
int tmpindex[MAX];
char elm[MAX][5] = {"elm1","elm2","elm3","elm4","elm5","elm6","elm7","elm8","elm9","elm10"};
char tmp[MAX][5] = {"tmp1","tmp2","tmp3","tmp4","tmp5","tmp6","tmp7","tmp8","tmp9","tmp10"};

void insertREF(char *s, enum StdType type, struct nodeType *link){
  int index = refTable.size;
   
  (refTable.size)++;
  strcpy(refTable.entries[index].name, s);
  refTable.entries[index].type = type;
  refTable.entries[index].link = link;
  //printf("table:0x%p, AddVariable:%s, valueType:%d\n", SymbolTable, s, type);
  //return &refTable.entries[index];

}
void deleteREF(char *s){
  int index = refTable.size;
  for(int i =0;i<index ; i++){
    if(!strcmp(refTable.entries[i].name,s)){
      //if found
      strcpy(refTable.entries[i].name, "");
      refTable.entries[i].type = 0;
      refTable.entries[i].link = 0;
      for(int j = i+1; j<index; j++){
        strcpy(refTable.entries[j-1].name, refTable.entries[j].name);
        refTable.entries[j-1].type = refTable.entries[j].type;
        refTable.entries[j-1].link = refTable.entries[j].link;
        if(j= index -1){
          strcpy(refTable.entries[j].name, "");
          refTable.entries[j].type = 0;
          refTable.entries[j].link = 0;
        } 
      } 
    }
  }
}

void DECREF(FILE* fptr){
    for(int i =0;i<refTable.size;i++){
      if(strcmp("",refTable.entries[i].name)){
        switch(refTable.entries[i].type){
          case TypeSEQ_I:
            fprintf(fptr, "DECREF_SEQ_I(%s);\n",refTable.entries[i].name);
            
            break;
          case TypeSEQ:
            switch(refTable.entries[i].link->typeNode->child->valueType){
              case TypeSEQ_I:
                fprintf(fptr, "DECREF_SEQ_SEQ_I(%s);\n",refTable.entries[i].name);
              break;
              
              case TypeSEQ:
                assert(0);//not implement;
              break;
            }
            break;
          default:
            assert(0); // not implement;
            break;
        }
      }
    }
}

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
      addVariable(tmp[i], node->valueType, node);
      tmpindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertres(struct nodeType* node){
  if(addVariable("res", node->valueType, node))
    return 1;
  else 
    return 0;
}

/**
* pfcheck is aimed to 
* 1. insert needed variables to symbol table,
* 2. label out the EXP that is return value of function.
* 3. make the etc...
*/
void pfcheck(struct nodeType* node){
  
  struct nodeType *child;
  if(node->child)
    child = node->child;
  switch(node->nodeType){
    
    //case NODE_NESL:{
     
    //  break;
    //}
    case NODE_FUNC:{
      node->isEndofFunction = 1;
      pfcheck(node->child->rsibling->rsibling);
      node->isparallel_rr = node->child->rsibling->rsibling->isparallel_rr;
      break;
    }
    case NODE_IFELSE:{
      node->isEndofFunction = node->parent->isEndofFunction;

      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      pfcheck(node->child->rsibling->rsibling);
      if(node->child->rsibling->isparallel_rr==1 
        || node->child->rsibling->rsibling->isparallel_rr ==1)
        node->isparallel_rr =1;
      break;
    }
    case NODE_ELSESTMT:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child); 
      node->isparallel_rr = node->child->isparallel_rr;
      break;
    }
    case NODE_THENSTMT:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child); 
      node->isparallel_rr = node->child->isparallel_rr;
      break;
    }
    case NODE_OP:{
      struct nodeType* LHS;
      struct nodeType* RHS;
      LHS = node->child;
      RHS = LHS->rsibling;
      if(node->parent->isEndofFunction){
        node->isEndofFunction = node->parent->isEndofFunction;
        LHS->isEndofFunction = node->isEndofFunction;
        RHS->isEndofFunction = node->isEndofFunction;
      }
      switch(node->op){
        case OP_PP:{
          //node->isEndofFunction = node->parent->isEndofFunction;
          pfcheck(LHS);
          pfcheck(RHS);
          
          if(LHS->op == OP_PP){
            LHS->inserttmp = 1;
            LHS->isEndofFunction = 0;
          }
          if(RHS->op == OP_PP){
            RHS->inserttmp = 1;
            RHS->isEndofFunction=0;
          }
          
          int index = inserttmp(node);
          assert(index!=-1);
          node->string = malloc(sizeof(char)*100);
          strcpy(node->string, tmp[index]);
          assert(node->string);
          break;
        }  
        case OP_BIND:{
          switch(RHS->nodeType){
            case NODE_APPLYBODY1:
              // not implement
              break;
            case NODE_APPLYBODY2:
              // {action: RBINDS}
              pfcheck(RHS);
              node->needcounter = RHS->needcounter;
              node->isparallel_rr = RHS->isparallel_rr;
              //assert(node->needcounter);
              if(node->needcounter){
                if(!findSymbol(node->table,"i"))
                  addVariable("i", TypeInt, node->parent->parent);
              }
              node->nodeType = GEN_APP2;
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, child->child->string);
              
              break;
            case NODE_APPLYBODY3:
              // {RBINDS|FILTER}
              pfcheck(RHS);
              node->needcounter = RHS->needcounter;
              node->isparallel_rr = RHS->isparallel_rr;
              
              if(node->needcounter)
                if(!findSymbol(node->table,"i"))
                addVariable("i", TypeInt, node->parent->parent);
              node->nodeType = GEN_APP3;
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, child->child->string);
              break;  
            case NODE_APPLYBODY4:
              // not implemented
              break;
            case NODE_SEQ_REF:
              //  node->nodeType
              // TODO what if a single exp with SEQ_REF
              //      which means to print the element? 
              //      
              // Ans. won't be here. cause'
              node->string = malloc(sizeof(char)*100);
              strcpy(node->string, node->child->child->string);
              node->nodeType = GEN_SEQ_REF;
              break;
            case NODE_FUNC_CALL:
            
            break;
            default:
              pfcheck(LHS);
              pfcheck(RHS);
              if(LHS->nodeType == NODE_TOKEN){
                assert(LHS->string);
                addVariable(LHS->string, LHS->valueType, LHS);
              }
              break;
           }
           break;
        }
        default:
          pfcheck(LHS);
          if(LHS->rsibling != LHS)
            pfcheck(LHS->rsibling);
          break;
      }
      break;
    }//end of case node->op
    
    case NODE_APPLYBODY2:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      node->needcounter = node->child->rsibling->needcounter;
      node->isparallel_rr = node->child->rsibling->isparallel_rr;
      break;  
    }
    case NODE_APPLYBODY3:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      node->needcounter = node->child->needcounter;
      node->isparallel_rr = node->child->isparallel_rr;
      break;
    }
     
    case NODE_FUNC_CALL:{
      if(!strcmp(node->child->string,"dist")){
        int index = inserttmp(node);
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, tmp[index]);
        node->inserttmp = 1;
      }
        pfcheck(node->child);
        pfcheck(node->child->rsibling);

    break;
    }

    case NODE_TUPLE:{
      int index = inserttmp(node);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, tmp[index]);
      node->inserttmp = 1;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
    break;
    }
    case NODE_NEW_SEQ:{
      
      // FIXME inserttmp in a nice way.
      // now it's dirty.
      node->table = node->parent->parent->parent->parent->table;
      int index = inserttmp(node);
      assert(node->parent->parent->parent->parent->nodeType == NODE_OP);
      
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, tmp[index]);
      assert(node->string);
      node->needcounter = 1;
      node->isparallel_rr =1;
      //node->inserttmp = 1;
      break;
    }
    case NODE_IN:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      if(node->child->rsibling->nodeType == NODE_NEW_SEQ ||
        node->child->rsibling->nodeType == NODE_FUNC_CALL){
        assert(node->child->rsibling->string);
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, node->child->rsibling->string);
        free(node->child->rsibling->string);
        node->needcounter = 1;
      }
      node->isparallel_rr = node->child->rsibling->isparallel_rr;
      if(node->isEndofFunction)
        assert(insertres(node->child));
      break;
    }
    case NODE_LET:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      if(node->child->isparallel_rr|node->child->rsibling->isparallel_rr)
        node->isparallel_rr =1;
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
      node->isEndofFunction = node->parent->isEndofFunction;
         //FIXME Exp
         //       |
         //     BINDS
         //     /   \
         //   ...   ... 
         // as a result:   
      
      pfcheck(node->child);
      if(node->isEndofFunction)
        node->child->lsibling->isEndofFunction =1;
      break;
    }
    case NODE_RBINDS:
    default:{
      struct nodeType* child = node->child;
      if(node->parent && node->child){
        node->isEndofFunction = node->parent->isEndofFunction;
        if(node->isEndofFunction)
          node->child->lsibling->isEndofFunction = 1;
      }
      if(child){
        do{
          pfcheck(child);
          if(child->needcounter)
            node->needcounter = 1;
          if(child->isparallel_rr)
            node->isparallel_rr = 1;
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
      
  }// end of switch node->nodeType
}

void phase1(FILE *fptr, struct nodeType* node){
  switch(node->nodeType){
  case NODE_TOKEN:
    if(node->tokenType == TOKE_ID){
      switch( node->valueType){
      case TypeInt:
        fprintf(fptr, "int %s;\n", node->string);
        break;
      case TypeFloat:
        fprintf(fptr, "float %s;\n", node->string);
        break;
      }
    }
    break;
  case NODE_OP:{
    switch(node->op){
    case OP_BIND:
      phase1(fptr, node->child);
      break;
    }
    break;
  }
  case NODE_PAIR:{
    phase1(fptr, node->child); 
  break;
  }
  case NODE_TUPLE:{
    //phase1(fptr, node->child); 
    switch (node->valueType){
    //case TypeTuple_I: 
    //  fprintf(fptr, "struct "); 
    //break;
    //case TypeTuple_I: 
    //  fprintf(fptr, "struct "); 
    //break;
    //case TypeTuple_I: 
    //  fprintf(fptr, "struct "); 
    //break;
    case TypeTuple_F: 
      fprintf(fptr, "struct Pair_F %s;\n", node->string); 
      phase1(fptr,node->child);
      phase1(fptr,node->child->rsibling);
    break;
    case TypeTuple: 
      fprintf(fptr, "struct Tuple %s;\n", node->string); 
      phase1(fptr,node->child);
      phase1(fptr,node->child->rsibling);
    break;
    }
  break;
  }
  
  }
}

void pfcodegen(FILE *fptr, struct nodeType* node){
  struct nodeType *child = node->child;
  switch(node->nodeType){
  case NODE_NESL:{
    
    //phase 1 : declare global variables;
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
        pfcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    
    fprintf(fptr, "#pragma global parallel\n");
    fprintf(fptr, "void myFunc1(){\n");
    for(int i =0;i<node->counts;i++){
      if(child->nodeType !=NODE_DATATYPE && child->nodeType != NODE_FUNC){
        pfcodegen(fptr, child);
      }
      child = child->rsibling;
    }
    fprintf(fptr, "}\n");
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
//      fprintf(fptr, "struct Sequence tmp;\n");

      pfcodegen(fptr, node->child->rsibling->rsibling);
      fprintf(fptr, "return res;\n");
      fprintf(fptr, "\n}\n");
      break;
    case TypeTuple:
      fprintf(fptr, "struct TypeTuple %s", node->string);
      pfcodegen(fptr,node->child);
      fprintf(fptr, "{\n");
      fprintf(fptr, "{\nstruct TypeTuple res;\n");
      //fprintf(fptr, "struct TypeTuple tmp;\n");
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
    if(node->isEndofFunction)
      DECREF(fptr);
      //fprintf(fptr, "endofFunction\n");
    fprintf(fptr, "}\n");
    break;

  case NODE_LET:
    dumpTable(fptr, node);
    pfcodegen(fptr, node->child);
    //fprintf(fptr, "\ntmp = ");
    pfcodegen(fptr, node->child->rsibling);
    //fprintf(fptr, ";\n");
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
      // if child is a variable, it will be process here.
      if(node->child->nodeType!=NODE_TOKEN)
        pfcodegen(fptr, node->child);
      if(node->child->rsibling->nodeType!=NODE_TOKEN)
        pfcodegen(fptr, node->child->rsibling);
      
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
        fprintf(fptr, "atomicAdd(REFCNT(%s, ",node->string);
        switch(node->valueType){
          case TypeSEQ_I:
            fprintf(fptr, "int), 1);\n\n");
            break;
          default:
            assert(0);//not implemented.
            break;
        }
        insertREF(node->string, node->valueType, node);
      }
      break;
    case OP_DIV:
      pfcodegen(fptr, node->child);
      fprintf(fptr, "/");
      pfcodegen(fptr, node->child->rsibling);
      break;
    }
    break;
  case NODE_SEQ_REF:{
    switch(node->valueType){
      case TypeInt:
        fprintf(fptr, "GET_ELEM_I(%s,%s,",node->string, node->child->string);
        pfcodegen(fptr, node->child->rsibling); // print index.
        fprintf(fptr, ");\n");
        break;
      case TypeSEQ_I:
        fprintf(fptr, "GET_ELEM_SEQ_I(%s,%s,",node->string, node->child->string);
        pfcodegen(fptr, node->child->rsibling); // print index.
        fprintf(fptr, ");\n");
        break;
      default:
        assert(0);
    }
    break;
  }
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
                      pfcodegen(fptr, phase3->child->rsibling);
                      fprintf(fptr, ";\n");
                      // int because paramtype is TypeSEQ_I
                      fprintf(fptr, "atomicAdd(REFCNT(res, int), 1);\n");
                      fprintf(fptr, "SET_ELEM_SEQ_I(res, %s, i);\n", phase2->string);
                      fprintf(fptr, "DECREF_SEQ_I(res);\n");
                      fprintf(fptr, "}\n");
                    }
                    
                    // struct sequence since it's TypeSEQ_I
                    // indicate that parent is TypeSEQ_SEQ_I
                    fprintf(fptr, "atomicAdd(REFCNT(%s, struct Sequence), 1);\n",phase2->string);
                    insertREF(phase2->string, phase2->valueType, phase2);
                    fprintf(fptr, "DECREF_SEQ_SEQ_I(%s);\n", phase1->string);
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
    pfcodegen(fptr, node->child->rsibling->child->rsibling->child);
    fprintf(fptr, "));\n");
    fprintf(fptr, "atomicAdd(REFCNT(%s, ",node->string);
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, "int), 1);\n");
        break;
    }
    insertREF(node->string, node->valueType, node);
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
    fprintf(fptr, "%s.a = ",node->string);
    pfcodegen(fptr, node->child);
    pfcodegen(fptr, node->child->rsibling);
    break;

  case NODE_PAIR:
    if(node->child->nodeType != NODE_TUPLE){
      fprintf(fptr, "(");
      assert(child);
      //pfcodegen(fptr,node->child);
      do{
        pfcodegen(fptr,node->child);
        child = child->rsibling;
      }while(child!=node->child);
      fprintf(fptr, ")");
    }
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
      //assert(entry);
      if(entry)
        fprintf(fptr, "%s", node->string);

    }

    }
    break;
  default:
    break;
  }
  return;
}


