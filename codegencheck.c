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
// TODO use sprintf;
char elm[MAX][6] = {"elm1","elm2","elm3","elm4","elm5","elm6","elm7","elm8","elm9","elm10",
  "elm11","elm12","elm13","elm14","elm15","elm16","elm17","elm18","elm19","elm20",
  "elm21","elm22","elm23","elm24","elm25","elm26","elm27","elm28","elm29","elm30"};
char tmp[MAX][6] = {"tmp1","tmp2","tmp3","tmp4","tmp5","tmp6","tmp7","tmp8","tmp9","tmp10",
  "tmp11","tmp12","tmp13","tmp14","tmp15","tmp16","tmp17","tmp18","tmp19","tmp20",
  "tmp21","tmp22","tmp23","tmp24","tmp25","tmp26","tmp27","tmp28","tmp29","tmp30"};
char app[MAX][6] = {"app1","app2","app3","app4","app5","app6","app7","app8","app9","app10",
  "app11","app12","app13","app14","app15","app16","app17","app18","app19","app20",
  "app21","app22","app23","app24","app25","app26","app27","app28","app29","app30"};
char add[MAX][6] = {"add1","add2","add3","add4","add5","add6","add7","add8","add9","add10",
  "add11","add12","add13","add14","add15","add16","add17","add18","add19","add20",
  "add21","add22","add23","add24","add25","add26","add27","add28","add29","add30"};
char sub[MAX][6] = {"sub1","sub2","sub3","sub4","sub5","sub6","sub7","sub8","sub9","sub10",
  "sub11","sub12","sub13","sub14","sub15","sub16","sub17","sub18","sub19","sub20",
  "sub21","sub22","sub23","sub24","sub25","sub26","sub27","sub28","sub29","sub30"};
char mul[MAX][6] = {"mul1","mul2","mul3","mul4","mul5","mul6","mul7","mul8","mul9","mul10",
  "mul11","mul12","mul13","mul14","mul15","mul16","mul17","mul18","mul19","mul20",
  "mul21","mul22","mul23","mul24","mul25","mul26","mul27","mul28","mul29","mul30"};
char ddiv[MAX][6] = {"div1","div2","div3","div4","div5","div6","div7","div8","div9","div10",
  "div11","div12","div13","div14","div15","div16","div17","div18","div19","div20",
  "div21","div22","div23","div24","div25","div26","div27","div28","div29","div30"};
char bol[MAX][6] = {"bol1","bol2","bol3","bol4","bol5","bol6","bol7","bol8","bol9","bol10",
  "bol11","bol12","bol13","bol14","bol15","bol16","bol17","bol18","bol19","bol20",
  "bol21","bol22","bol23","bol24","bol25","bol26","bol27","bol28","bol29","bol30"};
char let[MAX][6] = {"let1","let2","let3","let4","let5","let6","let7","let8","let9","let10",
  "let11","let12","let13","let14","let15","let16","let17","let18","let19","let20",
  "let21","let22","let23","let24","let25","let26","let27","let28","let29","let30"};
char _pp[MAX][6] = {"_pp1","_pp2","_pp3","_pp4","_pp5","_pp6","_pp7","_pp8","_pp9","_pp10",
  "_pp11","_pp12","_pp13","_pp14","_pp15","_pp16","_pp17","_pp18","_pp19","_pp20",
  "_pp21","_pp22","_pp23","_pp24","_pp25","_pp26","_pp27","_pp28","_pp29","_pp30"};
char ift[MAX][6] = {"ift1","ift2","ift3","ift4","ift5","ift6","ift7","ift8","ift9","ift10",
  "ift11","ift12","ift13","ift14","ift15","ift16","ift17","ift18","ift19","ift20",
  "_pp21","_pp22","_pp23","_pp24","_pp25","_pp26","_pp27","_pp28","_pp29","_pp30"};
char fcl[MAX][6] = {"fcl1","fcl2","fcl3","fcl4","fcl5","fcl6","fcl7","fcl8","fcl9","fcl10",
  "fcl11","fcl12","fcl13","fcl14","fcl15","fcl16","fcl17","fcl18","fcl19","fcl20",
  "_pp21","_pp22","_pp23","_pp24","_pp25","_pp26","_pp27","_pp28","_pp29","_pp30"};

void printAddREF(FILE *fptr, char* string, enum StdType type, struct nodeType* node){
  //insertREF(string, type, node);
  switch(type){
  //case TypeSEQ_I:
  //  fprintf(fptr, "atomicAdd(REFCNT(%s, int),1);\n",string);
  //  break;
  //case TypeSEQ_F:
  //  fprintf(fptr, "atomicAdd(REFCNT(%s, float),1);\n",string);
  //  break;
  case TypeSEQ:
    switch(node->typeNode->valueType){
    //case TypeTuple_F:
    //  fprintf(fptr, "atomicAdd(REFCNT(%s, struct Pair_F),1);\n",string);
    //  break;
    //case TypeTuple_I:
    //  fprintf(fptr, "atomicAdd(REFCNT(%s, struct Pair_I),1);\n",string);
    //  break;
    case TypeSEQ:
      fprintf(fptr, "atomicAdd(REFCNT(%s, struct Sequence),1);\n",string);
      break;
    
    default:
      assert(0);
    }
    break;
  case TypeTuple:{
    struct nodeType *Lchild = node->typeNode->child;
    struct nodeType *Rchild = Lchild->rsibling;
    while(Lchild->nodeType==NODE_PAIR) Lchild=Lchild->child;
    while(Rchild->nodeType==NODE_PAIR) Rchild=Rchild->child;
    if(containArray(Lchild))
      printAddREF(fptr, Lchild->string, Lchild->valueType, Lchild);
    if(containArray(Rchild))
      printAddREF(fptr, Rchild->string, Rchild->valueType, Rchild);
    break;
    }
  //case TypeTuple_SF:{
  //  struct nodeType *Lchild = node->child;
  //  while(Lchild->nodeType==NODE_PAIR||Lchild->nodeType==NODE_PATTERN) 
  //    Lchild=Lchild->child;
  //  printAddREF(fptr, Lchild->string, Lchild->valueType, Lchild);
  //  break;  
  //}
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
      //case TypeSEQ_F:
      //  fprintf(fptr, "DECREF_SEQ_F(%s);\n",refTable.entries[i].name);
      //  break;
      //case TypeSEQ_I:
      //  fprintf(fptr, "DECREF_SEQ_I(%s);\n",refTable.entries[i].name);
      //  break;
      case TypeSEQ:{
        // has different situation.
        struct nodeType *loopme;
        int types;
        //if(refTable.entries[i].link->typeNode->child)
        //  loopme = refTable.entries[i].link->typeNode->child;
        //else 
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
        //case TypeSEQ_I:
        //  fprintf(fptr, "_SEQ_I");
        //  break;
        //case TypeSEQ_F:
        //  fprintf(fptr, "_SEQ_F");
        //  break;
        case TypeSEQ:  
          assert(0);//not implement;
          break;
        case TypeFloat:
          fprintf(fptr, "_F");
        break;
        //case TypeTuple_F:
        //  fprintf(fptr, "_PAIR_F");
        //break;
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
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
      addVariable(varname, node->valueType, node);
      appindex[i]=1;
      if(node->nodeType==NODE_APPLYBODY3||node->nodeType==NODE_APPLYBODY2)
        node->table = tmp;
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

  case NODE_FUNC:{
    node->isEndofFunction = 1;
    pfcheck(node->child->rsibling->rsibling);
    node->isparallel_rr = node->child->rsibling->rsibling->isparallel_rr;
    if(node->isparallel_rr){
      struct SymTableEntry *entry = findSymbol(node->table,node->string);
      assert(entry);
      entry->link->isparallel_rr = 1;
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
        if(!findSymbol(node->table,"_i"))
          addVariable("_i", TypeInt, node->parent->parent);
      node->nodeType = GEN_APP3;
      node->string = malloc(sizeof(char)*100);
      node->valueType = RHS->valueType;
      node->typeNode = RHS->typeNode;
      switch(LHS->nodeType){
      case NODE_PATTERN:{
        struct SymTableEntry *ent = findSymbol(node->table, LHS->child->string);
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
        if(!findSymbol(node->table, LHS->string)){
          addVariable(LHS->string, LHS->valueType, LHS);
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
          if(!findSymbol(node->table,"_i"))
            addVariable("_i", TypeInt, node->parent->parent);
        node->nodeType = GEN_APP3;
        node->string = malloc(sizeof(char)*100);
        node->valueType = RHS->valueType;
        node->typeNode = RHS->typeNode;
        switch(LHS->nodeType){
        case NODE_PATTERN:{
          struct SymTableEntry *ent = findSymbol(node->table, LHS->child->string);
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
          if(!findSymbol(node->table, LHS->string)){
            addVariable(LHS->string, LHS->valueType, LHS);
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
    if(!findSymbol(node->table,"_len"))
      addVariable("_len", TypeInt, node);
    if(!findSymbol(node->table,"_i"))
      addVariable("_i", TypeInt, node);
    int idx = insertapp(node); 
    node->string = malloc(sizeof(char)*100);
    sprintf(node->string, "app%d",idx);
    
    if(!node->child->string){
      node->child->string = malloc(sizeof(char)*100);
      strcpy(node->child->string, "tmp");
      addVariable(node->child->string, node->child->valueType, node->child);
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
//    strcpy(node->string, fcl[index]);
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

    entry = findSymbol(node->table, LHS->string);
    if(entry){ if(entry->link->isparallel_rr) node->isparallel_rr=1; }

    break;
  }
  case PARAM_TUPLE:{
    struct nodeType *LHS = node->child;
    struct nodeType *RHS = LHS->rsibling;
    if(RHS->nodeType == NODE_TUPLE) RHS->nodeType = PARAM_TUPLE;
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
        struct SymTableEntry *entry = findSymbol(node->table,node->string);
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
    //case TypeTuple_F: 
    //  fprintf(fptr, "struct Pair_F %s;\n", node->string); 
    //  printGlobalVar(fptr,node->child);
    //  (fptr,node->child->rsibling);
    //  break;
    case TypeTuple: 
      fprintf(fptr, "struct Tuple %s;\n", node->string); 
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
  //case TypeSEQ_F:
  //  fprintf(fptr, "DECREF_SEQ_F(%s);\n",node->string);
  //  break;
  //case TypeSEQ_I:
  //  fprintf(fptr, "DECREF_SEQ_I(%s);\n",node->string);
  //  break;
  case TypeSEQ:{
    // has different situation.
    struct nodeType *loopme;
    int types;
    loopme = node->typeNode;

    fprintf(fptr, "DECREF_SEQ");
    int x=0;
    while(loopme->valueType ==TypeSEQ){
      fprintf(fptr, "_SEQ");
      loopme = loopme->typeNode;
      assert(loopme);
      if(x++==10) abort();//error;
    }
    switch(loopme->valueType){
    //case TypeSEQ_I:
    //  fprintf(fptr, "_SEQ_I");
    //  break;
    //case TypeSEQ_F:
    //  fprintf(fptr, "_SEQ_F");
    //  break;
    case TypeSEQ:  
      assert(0);//not implement;
      break;
    case TypeFloat:
      fprintf(fptr, "_F");
      break;
    //case TypeTuple_F:
    //  fprintf(fptr, "_PAIR_F");
    //  break;
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
