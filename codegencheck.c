#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"

#include "codegencheck.h"

struct RefTable refTable;
int elmindex[MAX];
int tmpindex[MAX];
int appindex[MAX];
int addindex[MAX];
int subindex[MAX];
int mulindex[MAX];
int divindex[MAX];
int bolindex[MAX];
int letindex[MAX];
int _ppindex[MAX];
int iftindex[MAX];
int fclindex[MAX];

void printAddREF(FILE *fptr, char* string, enum StdType type, struct nodeType* node){
  //insertREF(string, type, node);
  assert(string);
  struct nodeType* typer=node->child;
  if(node->typeNode){
    if(node->typeNode->child)
      typer = node->typeNode->child;
  }
  
  switch(type){
  case TypeSEQ:
    switch(typer->valueType){
    case TypeSEQ:
      fprintf(fptr, "atomicAdd(REFCNT(%s, struct Sequence),1);\n",string);
      break;
    case TypeInt:
      fprintf(fptr, "atomicAdd(REFCNT(%s, int),1);\n",string);
    break;
    case TypeFloat:
      fprintf(fptr, "atomicAdd(REFCNT(%s, float),1);\n",string);
    break;
    case TypeBool:
      fprintf(fptr, "atomicAdd(REFCNT(%s, bool),1);\n",string);
    break;
    case TypeChar:
      fprintf(fptr, "atomicAdd(REFCNT(%s, char),1);\n",string);
    break;
    case TypeTuple:
      fprintf(fptr, "atomicAdd(REFCNT(%s, ",string);
      printtype(fptr,typer); 
      fprintf(fptr, "),1);\n");
    break;
    default:
      assert(0);
    }
    break;
  case TypeTuple:{
    struct nodeType *Lchild = node->child;
    struct nodeType *Rchild = Lchild->rsibling;
    while(Lchild->nodeType==NODE_PAIR) Lchild=Lchild->child;
    while(Rchild->nodeType==NODE_PAIR) Rchild=Rchild->child;
    if(containArray(Lchild)){
      assert(Lchild->string);
      printAddREF(fptr, Lchild->string, Lchild->valueType, Lchild);
    }
    if(containArray(Rchild)){
      assert(Rchild->string);
      printAddREF(fptr, Rchild->string, Rchild->valueType, Rchild);
      }
    break;
    }
  default:
    assert(0);
    break;
  }
}

void insertREF(char *s, enum StdType type, struct nodeType *link){
  int index = refTable.size++;
  strcpy(refTable.entries[index].name, s);
  refTable.entries[index].type = type;
  refTable.entries[index].link = link;
  //printf("table:0x%p, AddVariable:%s, valueType:%d\n", SymbolTable, s, type);
  //return &refTable.entries[index];

}

int findREF(char *s){
  struct RefTableEntry *entry ;
  for(int i=refTable.size;i>=0;i--){
    entry = &refTable.entries[i];
    if(strcmp(s,entry->name)==0)
      return i;
  }
  return -1;
}

void deleteREF(int start, int end){

  for(int i =start;i<end ; i++){
    strcpy(refTable.entries[i].name, "");
    refTable.entries[i].type = 0;
    refTable.entries[i].link = 0;
    for(int j = i+1; j<end; j++){
      strcpy(refTable.entries[j-1].name, refTable.entries[j].name);
      refTable.entries[j-1].type = refTable.entries[j].type;
      refTable.entries[j-1].link = refTable.entries[j].link;
      if(j= end -1){
        strcpy(refTable.entries[j].name, "");
        refTable.entries[j].type = 0;
        refTable.entries[j].link = 0;
      } 
    } 
    refTable.size--;   
  }
}

void dumprefTable(){
  for(int i =0;i<refTable.size;i++){
    if(strcmp("",refTable.entries[i].name)){
      printf("%d, %s\n",i,refTable.entries[i].name); 
    }
  }
}

void DECREF(FILE* fptr,int n){
  int end = refTable.size;
  assert(end>=n);
  for(int i =refTable.size-n;i<end;i++){
    if(strcmp("",refTable.entries[i].name)&&refTable.entries[i].link->isEndofFunction!=1){
      switch(refTable.entries[i].type){
      case TypeSEQ:{
        // has different situation.
        struct nodeType *loopme;
        int types;
          loopme = refTable.entries[i].link->typeNode;
        
        fprintf(fptr, "DECREF_SEQ");
        int x=0;
        while(loopme->valueType ==TypeSEQ){
          fprintf(fptr, "_SEQ");
          loopme = loopme->typeNode;
          assert(loopme);
          if(x++==10) abort();//error;
        }
        switch(loopme->valueType){
        case TypeSEQ:  
          assert(0);//not implement;
          break;
        case TypeFloat:
          fprintf(fptr, "_F");
        break;
        default:
        assert(0); //not implement
        }
        fprintf(fptr, "(%s);\n", refTable.entries[i].name);
        break;}
      default:
        assert(0); // not implement;
        break;
      }
    }
    if(refTable.entries[i].link->isEndofFunction){
      refTable.entries[i].link->isEndofFunction=0;
    }
  }
  deleteREF(end-n,refTable.size);
}

int insertadd(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(addindex[i] ==0){
      sprintf(varname, "add%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      addindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertsub(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(subindex[i] ==0){
      sprintf(varname, "sub%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      subindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertmul(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(mulindex[i] ==0){
      sprintf(varname, "mul%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      mulindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertdiv(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(divindex[i] ==0){
      sprintf(varname, "ddiv%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      divindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertlet(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(letindex[i] ==0){
      sprintf(varname, "let%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      letindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}
int insertfcl(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(fclindex[i] ==0){
      sprintf(varname, "fcl%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      fclindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}
int insertift(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(iftindex[i] ==0){
      sprintf(varname, "ift%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      iftindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}
int insert_pp(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(_ppindex[i] ==0){
      sprintf(varname, "_pp%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      _ppindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}
int insertbol(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(bolindex[i] ==0){
      sprintf(varname, "bol%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      bolindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}
int insertelm(struct nodeType* node){
  char varname[100];
  for(int i =0; i<MAX; i++){
    if(elmindex[i] ==0){
      sprintf(varname, "elm%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      elmindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int inserttmp(struct nodeType* node){
  char varname[100];
  for(int i =0; i<=MAX; i++){
    if(tmpindex[i] ==0){
      sprintf(varname, "tmp%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      tmpindex[i]=1;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertapp(struct nodeType* node){
  char varname[100];
  struct SymTable *tmp = node->table;
  for(int i =0; i<=MAX; i++){
    if(appindex[i] ==0){
      if(node->nodeType==NODE_APPLYBODY3||node->nodeType==NODE_APPLYBODY2)
        node->table = node->table->parent;
      sprintf(varname, "app%d",i);
      addVariable(varname, node->valueType, node,REFERENCE);
      appindex[i]=1;
      if(node->nodeType==NODE_APPLYBODY3||node->nodeType==NODE_APPLYBODY2)
        node->table = tmp;
      return i;
    }else if(i==MAX) return -1;
  }
}

int insertres(struct nodeType* node){
  if(addVariable("res", node->valueType, node,REFERENCE))
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

  case NODE_FUNC:{
    struct nodeType *inputParam = node->child;
    
    node->isEndofFunction = 1;
    pfcheck(node->child->rsibling->rsibling);
    node->isparallel_rr = node->child->rsibling->rsibling->isparallel_rr;
    
    if(node->isparallel_rr){
      struct SymTableEntry *entry = findSymbol(node->table,node->string, REFERENCE);
      assert(entry);
      entry->link->isparallel_rr = 1;
    }
  
    if(inputParam->nodeType == NODE_TUPLE)
      inputParam->nodeType = FPARAM_TUPLE;
    if(inputParam->valueType == TypeTuple)
      pfcheck(inputParam); 
    
    node->tuplenode = inputParam; 
    break;
  }
  case FPARAM_TUPLE:{
    struct nodeType *LHS=node->child;
    struct nodeType *RHS=LHS->rsibling;
    int index = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",index);
    node->isvisited = 0;
    switch(LHS->nodeType){
    case NODE_TUPLE:
      LHS->nodeType= FPARAM_TUPLE;
      pfcheck(LHS);
      break;
    case NODE_TOKEN:
      pfcheck(LHS);
      break;
    default:
      assert(0);
      break;
    }
    switch(RHS->nodeType){
    case NODE_TUPLE:
      RHS->nodeType = FPARAM_TUPLE;
      pfcheck(RHS);
      break;
    case NODE_TOKEN:
      pfcheck(RHS);
      break;
    default:
      assert(0);
      break;
    }
    
    break;
  }
  case NODE_IFELSE:{
    struct nodeType* ifstmt = node->child;
    struct nodeType* thstmt = node->child->rsibling;
    struct nodeType* elstmt = node->child->rsibling->rsibling;
    node->isEndofFunction = node->parent->isEndofFunction;

    int index= insertift(node);
    assert(index!=-1);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "ift%d",index);
    assert(node->string);

    thstmt->string = malloc(sizeof(char)*100);
    strcpy(thstmt->string, node->string);
    assert(thstmt->string);

    elstmt->string = malloc(sizeof(char)*100);
    strcpy(elstmt->string, node->string);
    assert(elstmt->string);

    pfcheck(ifstmt);
    pfcheck(thstmt);
    pfcheck(elstmt);
    if(thstmt->isparallel_rr==1 
       || elstmt->isparallel_rr ==1)
      node->isparallel_rr =1;

    break;
  }
  case NODE_IFSTMT:{

    //node->isEndofFunction = node->parent->isEndofFunction;
    pfcheck(node->child);
    node->isparallel_rr = node->child->isparallel_rr;

    break;}
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
  case NODE_ASSIGN:{
    struct nodeType* LHS;
    struct nodeType* RHS;
    LHS = node->child;
    LHS->infilter = node->infilter;
    RHS = LHS->rsibling;
    RHS->infilter = node->infilter;
    if(node->parent->isEndofFunction){
      node->isEndofFunction = node->parent->isEndofFunction;
      LHS->isEndofFunction = node->isEndofFunction;
      RHS->isEndofFunction = node->isEndofFunction;
    }
    while(RHS->nodeType == NODE_PAIR) RHS= RHS->child;
    while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
    switch(RHS->nodeType){
    case NODE_APPLYBODY1:
      abort();
      // not implement
      break;
    case NODE_APPLYBODY2:
      // {action: RBINDS}
      pfcheck(RHS);
      node->isparallel_rr = RHS->isparallel_rr;
      break;
    case NODE_APPLYBODY3:
      // {RBINDS|FILTER}
      pfcheck(RHS);
      node->needcounter = RHS->needcounter;
      node->isparallel_rr = RHS->isparallel_rr;

      if(node->needcounter)
        if(!findSymbol(node->table,"_i", REFERENCE))
          addVariable("_i", TypeInt, node->parent->parent,REFERENCE);
      node->nodeType = GEN_APP3;
      node->string = malloc(sizeof(char)*100);
      node->valueType = RHS->valueType;
      node->typeNode = RHS->typeNode;
      switch(LHS->nodeType){
      case NODE_PATTERN:{
        struct SymTableEntry *ent = findSymbol(node->table, LHS->child->string, REFERENCE);
        if(ent){
          if(ent->link->typeNode){
            ent->link->typeNode = RHS->typeNode;
          }
        }
        strcpy(node->string, LHS->child->string);
        LHS->child->typeNode = RHS;
        break;}
      case NODE_TOKEN:
        strcpy(node->string, LHS->string);
        LHS->typeNode = RHS->typeNode;
        LHS->valueType = RHS->valueType;
        break;
      }
      return;  
    case NODE_APPLYBODY4:
      abort();// not implemented
      break;
    case NODE_TUPLE: //RHS->tuple
      RHS->nodeType = RHS_TUPLE;
      pfcheck(RHS);
      assert(RHS->string);
      break;
    default:
      pfcheck(LHS);
      pfcheck(RHS);
      if(LHS->isparallel_rr || RHS->isparallel_rr)node->isparallel_rr=1;
      if(LHS->nodeType == NODE_TOKEN){
        assert(LHS->string);
        if(!findSymbol(node->table, LHS->string, REFERENCE)){
          addVariable(LHS->string, LHS->valueType, LHS,REFERENCE);
        }
      }
      break;
    }// end of RHS->nodeType

    while(LHS->nodeType == NODE_PATTERN) LHS= LHS->child;
    while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
    switch(LHS->nodeType){
    case NODE_TUPLE:  
      LHS->nodeType = LHS_TUPLE;
      pfcheck(LHS);
      assert(LHS->string);
      break;
    default:
      pfcheck(LHS);
      break;
    }

    LHS=node->child;
    RHS=node->child->rsibling;


    break;
  } // end of OP_BIND

  case NODE_OP:{
    struct nodeType* LHS;
    struct nodeType* RHS;
    LHS = node->child;
    LHS->infilter = node->infilter;
    RHS = LHS->rsibling;
    RHS->infilter = node->infilter;
    if(node->parent->isEndofFunction){
      node->isEndofFunction = node->parent->isEndofFunction;
      LHS->isEndofFunction = node->isEndofFunction;
      RHS->isEndofFunction = node->isEndofFunction;
    }
    switch(node->op){
    case OP_LT:
    case OP_LE:
    case OP_EQ:
    case OP_GT:
    case OP_GE:{
      if(!node->infilter){
        int index= insertbol(node);
        assert(index!=-1);
        node->string = malloc(sizeof(char)*100);
        sprintf(node->string, "bol%d",index);
        assert(node->string);
        //addVariable(node->string, node->valueType, node);
      }
      pfcheck(LHS);
      pfcheck(RHS);
      break;}
    case OP_ADD:{

      if(!node->infilter){
      int index= insertadd(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "add%d",index);
      assert(node->string);
      }
      pfcheck(LHS);
      pfcheck(RHS);
      break;
    }case OP_SUB:{

      if(!node->infilter){
      int index= insertsub(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "sub%d",index);
      assert(node->string);
      }
      pfcheck(LHS);
      pfcheck(RHS);
      break;
    }case OP_MUL:{

      if(!node->infilter){
      int index= insertmul(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "mul%d",index);
      assert(node->string);
      }
      pfcheck(LHS);
      pfcheck(RHS);
      break;
    }case OP_DIV:{

      if(!node->infilter){
      int index= insertdiv(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "ddiv%d",index);
      assert(node->string);
      }
      pfcheck(LHS);
      pfcheck(RHS);
      break;
    }
    case OP_UMINUS:
    case OP_SHARP:{

      if(!node->infilter){
      int index= inserttmp(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "tmp%d",index);
      assert(node->string);
      }
      pfcheck(LHS);

      break;}
    case OP_UPT:{
      if(!node->infilter){
      int index= inserttmp(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "tmp%d",index);
      assert(node->string);
      }
      pfcheck(LHS);
      pfcheck(RHS);     
      break;
    }
    case OP_PP:{
      //node->isEndofFunction = node->parent->isEndofFunction;
      if(LHS->nodeType !=NODE_SEQ)
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
      node->typeNode = LHS->typeNode;  

      int index= insert_pp(node);
      assert(index!=-1);
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "_pp%d",index);
      assert(node->string);

      break;
    }  
    case OP_BIND:{
      assert(0);
      while(RHS->nodeType == NODE_PAIR) RHS= RHS->child;
      while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
      switch(RHS->nodeType){
      case NODE_APPLYBODY1:
        abort();
        // not implement
        break;
      case NODE_APPLYBODY2:
        /* {action: RBINDS} */
        pfcheck(RHS);
        node->isparallel_rr = RHS->isparallel_rr;
        break;
      case NODE_APPLYBODY3:
        /* {RBINDS|FILTER} */
        pfcheck(RHS);
        node->needcounter = RHS->needcounter;
        node->isparallel_rr = RHS->isparallel_rr;

        if(node->needcounter)
          if(!findSymbol(node->table,"_i", FORCEDECLARE))
            addVariable("_i", TypeInt, node->parent->parent,FORCEDECLARE);
        node->nodeType = GEN_APP3;
        node->string = malloc(sizeof(char)*100);
        node->valueType = RHS->valueType;
        node->typeNode = RHS->typeNode;
        switch(LHS->nodeType){
        case NODE_PATTERN:{
          struct SymTableEntry *ent = findSymbol(node->table, LHS->child->string, REFERENCE);
          if(ent){
            if(ent->link->typeNode){
              ent->link->typeNode = RHS->typeNode;
            }
          }
          strcpy(node->string, LHS->child->string);
          LHS->child->typeNode = RHS;
          break;}
        case NODE_TOKEN:
          strcpy(node->string, LHS->string);
          LHS->typeNode = RHS;
          break;
        }
        return;  
      case NODE_APPLYBODY4:
        abort();// not implemented
        break;
      case NODE_TUPLE: //RHS->tuple
        RHS->nodeType = RHS_TUPLE;
        pfcheck(RHS);
        assert(RHS->string);
        break;
      default:
        pfcheck(LHS);
        pfcheck(RHS);
        if(LHS->isparallel_rr || RHS->isparallel_rr)node->isparallel_rr=1;
        if(LHS->nodeType == NODE_TOKEN){
          assert(LHS->string);
          if(!findSymbol(node->table, LHS->string, REFERENCE)){
            addVariable(LHS->string, LHS->valueType, LHS,REFERENCE);
          }
        }
        break;
      }// end of RHS->nodeType
      
      while(LHS->nodeType == NODE_PATTERN) LHS= LHS->child;
      while(LHS->nodeType == NODE_PAIR) LHS= LHS->child;
      switch(LHS->nodeType){
      case NODE_TUPLE:  
        LHS->nodeType = LHS_TUPLE;
        pfcheck(LHS);
        assert(LHS->string);
        break;
      default:
        pfcheck(LHS);
        break;
      }

      LHS=node->child;
      RHS=node->child->rsibling;


      break;
    } // end of OP_BIND
    default:
      pfcheck(LHS);
      if(LHS->rsibling != LHS)
        pfcheck(LHS->rsibling);
      break;
    }
    break;
  }//end of case node->op
  case NODE_ACTION_TUPLE:{
    //if there is another tuple, transform and pfcheck it.
    struct nodeType *child = node->child;
    do{
      if(child->nodeType == NODE_TUPLE){
        child->nodeType = NODE_ACTION_TUPLE;
        pfcheck(child);
      }
      child = child->rsibling;
    }while(child!=node->child);

    int index = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",index);
    node->inserttmp = 1;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);
    break;
  }
  case NODE_APPLYBODY2:{
    node->isEndofFunction = node->parent->isEndofFunction;

    if(node->child->nodeType == NODE_TUPLE)
      node->child->nodeType = NODE_ACTION_TUPLE;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);

    node->needcounter = 1;
    node->isparallel_rr = 1;
    if(!findSymbol(node->table,"_len", FORCEDECLARE))
      addVariable("_len", TypeInt, node, FORCEDECLARE);
    if(!findSymbol(node->table,"_i", FORCEDECLARE))
      addVariable("_i", TypeInt, node, FORCEDECLARE);
    int idx = insertapp(node); 
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "app%d",idx);
    
    if(!node->child->string){
      node->child->string = malloc(sizeof(char)*100);
      strcpy(node->child->string, "tmp");
      addVariable(node->child->string, node->child->valueType, node->child, FORCEDECLARE);
    }
    printTree(node,0);
    break;  
  }
  case NODE_APPLYBODY3:{
    node->isEndofFunction = node->parent->isEndofFunction;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);
    node->needcounter = node->child->needcounter;
    node->isparallel_rr = node->child->isparallel_rr;
    if(node->parent->op!=NODE_ASSIGN){
      int idx = insertapp(node); 
      node->string = malloc(sizeof(char)*100);
      sprintf(node->string, "app%d",idx);
    }
    break;
  }
  case NODE_FUNC_CALL:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;
    struct SymTableEntry *entry;
    node->isEndofFunction = node->parent->isEndofFunction; 
    int index= insertfcl(node);
    assert(index!=-1);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "fcl%d",index);
    assert(node->string);

    node->isEndofFunction = node->parent->isEndofFunction;

    if(!strcmp(LHS->string,"dist")||node->parent->nodeType == NODE_NESL){
      issrand = 1;
    }

    while (RHS->nodeType ==NODE_PAIR) RHS= RHS->child;
    while (RHS->nodeType ==NODE_TUPLE) RHS->nodeType = PARAM_TUPLE;
    pfcheck(LHS);
    pfcheck(RHS);
    if(LHS->isparallel_rr || RHS->isparallel_rr ) node->isparallel_rr=1;

    entry = findSymbol(node->table, LHS->string, REFERENCE);
    if(entry){ if(entry->link->isparallel_rr) node->isparallel_rr=1; }

    break;
  }
  case PARAM_TUPLE:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;
    //if(RHS->nodeType == NODE_TUPLE) RHS->nodeType = PARAM_TUPLE;
    if(RHS->nodeType == NODE_TUPLE) RHS->nodeType = RHS_TUPLE;
    pfcheck(LHS);
    pfcheck(RHS);
    break;}

  case RB_TUPLE:
  case LHS_TUPLE:{
    struct nodeType *LHS= node->child;
    struct nodeType *RHS= LHS->rsibling;

    int index = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",index);
    
    while(LHS->nodeType == NODE_PAIR) LHS=LHS->child;
    while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
    if(LHS->nodeType==NODE_TUPLE) LHS->nodeType = LHS_TUPLE;
    if(RHS->nodeType==NODE_TUPLE) RHS->nodeType = LHS_TUPLE;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);
    break;
  }
  case RHS_TUPLE:{
    struct nodeType *LHS= node->child;
    struct nodeType *RHS= LHS->rsibling;

    int index = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",index);
    
    while(LHS->nodeType == NODE_PAIR) LHS=LHS->child;
    while(RHS->nodeType == NODE_PAIR) RHS=RHS->child;
    if(LHS->nodeType==NODE_TUPLE) LHS->nodeType = RHS_TUPLE;
    if(RHS->nodeType==NODE_TUPLE) RHS->nodeType = RHS_TUPLE;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);
    break;
  }
  case ELEM_TUPLE:
  case NODE_TUPLE:{
    int index = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",index);
    
    node->inserttmp = 1;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);
    break;
  }
  case NODE_SEQ_TUPLE:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;

    node->string = malloc(sizeof(char)*100);
    strcpy(node->string, node->parent->string);

    //if(RHS->nodeType == NODE_SEQ_TUPLE){
    //  pfcheck(RHS);
    //}
    pfcheck(LHS);
    pfcheck(RHS);
    break;
  }
  case NODE_SEQ:{
    pfcheck(node->child); 
    int idx = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",idx);
    node->counts = 1; // only one child;
  }
  case NODE_NEW_SEQ:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;

    int idx = inserttmp(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "tmp%d",idx);

    LHS->paramcount = 0;
    RHS->paramcount = 1;
    if(RHS->nodeType == NODE_SEQ_TUPLE){
      //pfcheck(RHS);
      node->counts = RHS->counts+1;
    }
    else{
      node->counts = 2;
    }
    pfcheck(LHS);
    pfcheck(RHS);

    break;
  }
  case NODE_IN:{
    //node->isEndofFunction = node->parent->isEndofFunction;
    pfcheck(node->child);
    pfcheck(node->child->rsibling);

    if(node->child->rsibling->nodeType == NODE_NEW_SEQ ){
      assert(node->child->rsibling->string);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, node->child->rsibling->string);
      node->needcounter = 1;
    }
    node->isparallel_rr = node->child->rsibling->isparallel_rr;
    break;
  }
  case NODE_LET:{
    struct nodeType* LHS = node->child;
    struct nodeType* RHS = node->child->rsibling;

    node->isEndofFunction = node->parent->isEndofFunction;
    RHS->nodeType = NODE_LETRET;
    pfcheck(LHS);
    pfcheck(RHS);
    if(LHS->isparallel_rr||RHS->isparallel_rr)
      node->isparallel_rr =1;

    node->string = malloc(sizeof(char)*100);
    strcpy(node->string, RHS->string);
    break;
  }
  case NODE_BIND:{
    struct nodeType *child = node->child;
    int counts=0;
    if(child!=0){
      do{
        counts++;
        pfcheck(child);
        if(child->isparallel_rr)
          node->isparallel_rr = 1;
        child = child->rsibling;
      }while(child!=node->child);
    }
    break;
  }
  case NODE_LETRET:{
    
    //node->isEndofFunction = node->parent->isEndofFunction;
    pfcheck(node->child);
    if(node->child->isparallel_rr)
      node->isparallel_rr = 1;
    struct SymTable *table = node->table;
    if(table->parent);
    node->table = table->parent;
    int index= insertlet(node);
    node->table = table;
    assert(index!=-1);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "let%d",index);
    assert(node->string);
    break;
  }
  case NODE_SEQ_REF:{
    int index = insertelm(node);
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "elm%d",index);
    assert(node->string);
    pfcheck(node->child);
    assert(node->child->string);
    pfcheck(node->child->rsibling);
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
  case NODE_RBINDS:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = node->child->rsibling;
    struct nodeType* child = node->child;
    int count=0;

    if(node->parent && node->child){
      node->isEndofFunction = node->parent->isEndofFunction;
      if(node->isEndofFunction)
        node->child->lsibling->isEndofFunction = 1;
    }
    if(child){
      do{
        count++;
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

  case NODE_FILTER:
    node->child->infilter =1;   
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

/*declare global variables.*/
void printGlobalVar(FILE *fptr, struct nodeType* node){
  switch(node->nodeType){
  case NODE_TOKEN:
    if(node->tokenType == TOKE_ID){
      switch( node->valueType){
      case TypeInt:{
        assert(node->string);
        struct SymTableEntry *entry = findSymbol(node->table,node->string, REFERENCE);
        fprintf(fptr, "int %s;\n", node->string);
        if(entry){
          entry->isParam =1; 
        }
        else assert(0);// semantic check pass error
        break;}
      case TypeFloat:
        fprintf(fptr, "float %s;\n", node->string);
        break;
      }
    }
    break;
  case NODE_OP:{
    switch(node->op){
    case OP_BIND:
      printGlobalVar(fptr, node->child);
      break;
    }
    break;
  }
  case NODE_PAIR:{
    printGlobalVar(fptr, node->child); 
    break;
  }
  case NODE_TUPLE:{
    switch (node->valueType){
    case TypeTuple: 
      //fprintf(fptr, "struct Tuple %s;\n", node->string); 
      assert(0);
      printGlobalVar(fptr,node->child);
      printGlobalVar(fptr,node->child->rsibling);
      break;
    }
    break;
  }

  }
}

void printDECREF(FILE *fptr, struct nodeType *node){
  
  switch(node->valueType){
  case TypeSEQ:{
    struct nodeType *loopme;
    int types;
    loopme = node->typeNode->child;

    fprintf(fptr, "DECREF_SEQ_");
    int x=0;
    while(loopme->valueType ==TypeSEQ){
      fprintf(fptr, "S");
      loopme = loopme->typeNode->child;
      assert(loopme);
      if(x++==10) abort();//error;
    }
    switch(loopme->valueType){
    case TypeSEQ:  
      assert(0);//not implement;
      break;
    case TypeInt:
      fprintf(fptr, "I");
      break;
    case TypeFloat:
      fprintf(fptr, "F");
      break;
    case TypeTuple:
      gentypes(fptr, loopme);
      break;
    default:
      assert(0); //not implement
    }
    fprintf(fptr, "(%s);\n", node->string);
    break;}
  default:
    assert(0); // not implement;
    break;
  }
}
