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
char elm[MAX][6] = {"elm1","elm2","elm3","elm4","elm5","elm6","elm7","elm8","elm9","elm10",
        "elm11","elm12","elm13","elm14","elm15","elm16","elm17","elm18","elm19","elm20",
        "elm21","elm22","elm23","elm24","elm25","elm26","elm27","elm28","elm29","elm30"};
char tmp[MAX][6] = {"tmp1","tmp2","tmp3","tmp4","tmp5","tmp6","tmp7","tmp8","tmp9","tmp10",
"tmp11","tmp12","tmp13","tmp14","tmp15","tmp16","tmp17","tmp18","tmp19","tmp20",
"tmp21","tmp22","tmp23","tmp24","tmp25","tmp26","tmp27","tmp28","tmp29","tmp30"};
char app[MAX][6] = {"app1","app2","app3","app4","app5","app6","app7","app8","app9","app10",
        "app11","app12","app13","app14","app15","app16","app17","app18","app19","app20",
        "app21","app22","app23","app24","app25","app26","app27","app28","app29","app30"};
void printAddREF(FILE *fptr, char* string, enum StdType type, struct nodeType* node){
  insertREF(string, type, node);
  if(type == TypeSEQ_I)
  fprintf(fptr, "atomicAdd(REFCNT(%s, int),1);\n",string);
  else
  fprintf(fptr, "atomicAdd(REFCNT(%s, struct Sequence),1);\n",string);
}

void insertREF(char *s, enum StdType type, struct nodeType *link){
  int index = refTable.size++;
  strcpy(refTable.entries[index].name, s);
  refTable.entries[index].type = type;
  refTable.entries[index].link = link;
  //printf("table:0x%p, AddVariable:%s, valueType:%d\n", SymbolTable, s, type);
  //return &refTable.entries[index];

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
      if(strcmp("",refTable.entries[i].name)){
        switch(refTable.entries[i].type){
          case TypeSEQ_I:
            fprintf(fptr, "DECREF_SEQ_I(%s);\n",refTable.entries[i].name);
            break;
          case TypeSEQ:{
            // has different situation.
            int types;
            if(refTable.entries[i].link->typeNode->child)
              types = refTable.entries[i].link->typeNode->child->valueType;
            else 
              types = refTable.entries[i].link->typeNode->valueType;
            switch(types){
              case TypeSEQ_I:
                fprintf(fptr, "DECREF_SEQ_SEQ_I(%s);\n",refTable.entries[i].name);
              break;
              
              case TypeSEQ:
                assert(0);//not implement;
              break;
            }
            break;}
          default:
            assert(0); // not implement;
            break;
        }
      }
    }
    deleteREF(end-n,refTable.size);
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

int insertapp(struct nodeType* node){
  for(int i =0; i<=MAX; i++){
    if(appindex[i] ==0){
      if(node->nodeType==NODE_APPLYBODY2)
        node->table = node->table->parent;
      addVariable(app[i], node->valueType, node);
      appindex[i]=1;
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
          node->typeNode = LHS->typeNode;  
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
              switch(LHS->nodeType){
              case NODE_PATTERN:
                if(LHS->child->string)
                  strcpy(RHS->string,LHS->child->string);
              break;
              case NODE_TOKEN:
                if(LHS->string)
                  strcpy(RHS->string,LHS->string);
              break;
              default:
                assert(0);//not implement.
              break;
              }
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
      strcpy(node->string, tmp[index]);
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
      //node->needcounter = node->child->rsibling->needcounter;
      //node->isparallel_rr = node->child->rsibling->isparallel_rr;
      
      node->needcounter = 1;
      node->isparallel_rr = 1;
      if(!findSymbol(node->table,"_len"))
        addVariable("_len", TypeInt, node);
      if(!findSymbol(node->table,"_i"))
        addVariable("_i", TypeInt, node);
      int idx = insertapp(node); 
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, app[idx]);
      //node->string = malloc(sizeof(char)*100);
      //strcpy(node->string, child->child->string);
              
      struct nodeType * returnchild = newNode(NODE_APPRET);
      returnchild->valueType = node->child->valueType;
      returnchild->typeNode = node->child->typeNode;
      returnchild->table = node->child->table;
      returnchild->string = malloc(sizeof(char)*100);
      strcpy(returnchild->string, "tmp");
      addChild(node, returnchild);
      addVariable(returnchild->string, returnchild->valueType, returnchild);
      printTree(node,0);
      break;  
    }
    case NODE_APPLYBODY3:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      node->needcounter = node->child->needcounter;
      node->isparallel_rr = node->child->isparallel_rr;

      struct nodeType * returnchild = newNode(NODE_APPRET);
      returnchild->valueType = node->child->rsibling->child->valueType;
      returnchild->typeNode = node->child->rsibling->child->typeNode;
      node->typeNode = node->child->rsibling->child->typeNode;
      returnchild->string = malloc(sizeof(char)*100);
      strcpy(returnchild->string, "tmp");
      addChild(node, returnchild);
      break;
    }
     
    case NODE_FUNC_CALL:{
      node->isEndofFunction = node->parent->isEndofFunction;
      if(!strcmp(node->child->string,"dist")||
        node->parent->nodeType == NODE_NESL){
        int index = inserttmp(node);
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, tmp[index]);
        node->inserttmp = 1;
        issrand = 1;
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
    case NODE_SEQ_TUPLE:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;
      
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, node->parent->string);

      if(RHS->nodeType == NODE_SEQ_TUPLE){
        pfcheck(RHS);
      }
    
    break;
    } 
    case NODE_NEW_SEQ:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;

      int idx = inserttmp(node);
      node->string = malloc(sizeof(char)*100);
      strcpy(node->string, tmp[idx]);
      
      LHS->paramcount = 0;
      RHS->paramcount = 1;
      if(RHS->nodeType == NODE_SEQ_TUPLE){
        pfcheck(RHS);
        node->counts = RHS->counts+1;
      }
      else{
        node->counts = 2;
      }
      // FIXME inserttmp in a nice way.
      // now it's dirty.
      //node->table = node->parent->parent->parent->parent->table;
      //int index = inserttmp(node);
      //assert(node->parent->parent->parent->parent->nodeType == NODE_OP);
      //
      //assert(index!=-1);
      //node->string = malloc(sizeof(char)*100);
      //strcpy(node->string, tmp[index]);
      //assert(node->string);
      //node->needcounter = 1;
      //node->isparallel_rr =1;
      //node->inserttmp = 1;
      break;
    }
    case NODE_IN:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      
      if(node->child->rsibling->nodeType == NODE_NEW_SEQ ){
        assert(node->child->rsibling->string);
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, node->child->rsibling->string);
        //free(node->child->rsibling->string);
        node->needcounter = 1;
      }
      node->isparallel_rr = node->child->rsibling->isparallel_rr;
      break;
    }
    case NODE_LET:{
      node->isEndofFunction = node->parent->isEndofFunction;
      pfcheck(node->child);
      pfcheck(node->child->rsibling);
      if(node->child->isparallel_rr||node->child->rsibling->isparallel_rr)
        node->isparallel_rr =1;
      node->child->rsibling->nodeType = NODE_LETRET;
      //if(node->isEndofFunction)
      //  assert(insertres(node->child));
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
      if(LHS->string){
        node->string = malloc(sizeof(char)*100);
        strcpy(node->string, RHS->string);
      }
      
      if(node->parent->nodeType == NODE_APPLYBODY2){
        struct nodeType* sourceArrays = newNode(NODE_SRCARR);
        struct nodeType* freeVars = newNode(NODE_FreeVars);
        struct nodeType *arrchild, *varchild;
        addChild(node->parent, sourceArrays);
        addChild(node->parent, freeVars);
        child = node->child;
        for(int i =0; i<count;i++){
          arrchild = child->child->rsibling;
          varchild = child->child;
          if(arrchild->lsibling == varchild)
            arrchild->lsibling = arrchild;
          if(arrchild->rsibling == varchild)
            arrchild->rsibling = arrchild;
          if(varchild->rsibling == arrchild)
            varchild->rsibling = varchild;
          if(varchild->lsibling == arrchild)
            varchild->lsibling = varchild;
          addChild(sourceArrays, arrchild);
          addChild(freeVars, varchild);
          freeVars->counts++;
          //child->child->rsibling = child->child;
          //child->child->lsibling = child->child;
          //child->child->rsibling->rsibling = child->child->rsibling;
          //child->child->rsibling->lsibling = child->child->rsibling;

          child = child->rsibling;
        }
        node->lsibling->rsibling = node->rsibling;
        node->rsibling->lsibling = node->lsibling;
        printTree(node->parent,0);
      }
      else if (node->parent->nodeType == NODE_APPLYBODY3){
        struct nodeType* sourceArrays = newNode(NODE_SRCARR);
        struct nodeType* freeVars = newNode(NODE_FreeVars);
        struct nodeType *arrchild, *varchild;
        addChild(node->parent, sourceArrays);
        addChild(node->parent, freeVars);
        child = node->child;
        for(int i =0; i<count;i++){
          arrchild = child->child->rsibling;
          varchild = child->child;
          if(arrchild->lsibling == varchild)
            arrchild->lsibling = arrchild;
          if(arrchild->rsibling == varchild)
            arrchild->rsibling = arrchild;
          if(varchild->rsibling == arrchild)
            varchild->rsibling = varchild;
          if(varchild->lsibling == arrchild)
            varchild->lsibling = varchild;
          addChild(sourceArrays, arrchild);
          addChild(freeVars, varchild);
          freeVars->counts++;
          //child->child->rsibling = child->child;
          //child->child->lsibling = child->child;
          //child->child->rsibling->rsibling = child->child->rsibling;
          //child->child->rsibling->lsibling = child->child->rsibling;
          
          child = child->rsibling;
        }
        node->lsibling->rsibling = node->rsibling;
        node->rsibling->lsibling = node->lsibling;
        node->parent->child= sourceArrays;
        printTree(node->parent,0);
      }

      //assert(0);
      //for(

      break;
    }

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

