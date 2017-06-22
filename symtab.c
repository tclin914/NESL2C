#include "node.h"
#include "symtab.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}



struct SymTable *rootTable;
struct FuncTable *funcTable;

struct nodeType * findtype(struct nodeType *node){
  int i=0;
  if(typeTable->size ==0) return 0;
  for(i=0;i<typeTable->size;i++){
    if(isSameType( typeTable->link[i], node))
      return typeTable->link[i];
  }
  return 0;
}

struct FuncTableEntry * findFuction(char *s){
  int size = funcTable->size;
  if(size ==0) return 0;
  for(int i=0; i<size;i++){
    if(!strcmp(funcTable->entries[i].name, s))
      return &funcTable->entries[i];
  }
  return 0;
}

struct SymTable * newSymTable(struct SymTable * parent){
  struct SymTable *table = (struct SymTable*)malloc(sizeof(struct SymTable));
  table->size = 0;
  for(int i=0;i<100;i++){
    strcpy(table->entries[i].name, "");
  }
  table->parent = parent;
  return table;
}

struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s, int mode) {
    //struct SymTableEntry * entry;
    assert(s);
    
    if(SymbolTable->size !=0){
      for(int i=0; i<SymbolTable->size; i++) {
        if(strcmp(s, SymbolTable->entries[i].name) == 0) {
          return &SymbolTable->entries[i];
        }

      }
    }
    switch(mode){
    case FORCEDECLARE:
      return 0;
      break;
    case REFERENCE:{
      if(SymbolTable->parent !=0 && mode == REFERENCE)
        return findSymbol(SymbolTable->parent, s, mode);  
      else
        return 0;
    }
    case NOROOT:{
      if(SymbolTable->parent != rootTable){
        return findSymbol(SymbolTable->parent, s, mode);  
      }
      else return 0;
    }
    default: assert(0);
    }
    return 0;// impossible
}

void addtype(struct nodeType *node, enum StdType type){
  assert(node->valueType==type);
  assert(node->valueType==TypeTuple);
  int i =0;
  if(!findtype(node)){
    typeTable->link[(typeTable->size)++] = node;
  }
}


struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link, int mode) {
    struct SymTable *SymbolTable = link->table;
    
    if(findSymbol(link->table, s, mode) != 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        //exit(0);
    }

    int index = SymbolTable->size;
    (SymbolTable->size)++;

    strcpy(SymbolTable->entries[index].name, s);
    SymbolTable->entries[index].type = type;
    SymbolTable->entries[index].link = link;
    SymbolTable->entries[index].isParam = link->isParam;
    //printf("dump entry:%d, name:%s, type:%d, link:%x\n", index, 
    //    SymbolTable->entries[index].type,
    //    SymbolTable->entries[index].link);
    
    if(type == TypeTuple) addtype(link,type);
    printf("table:0x%p, AddVariable:%s, valueType:%d\n", SymbolTable, s, type);
    return &SymbolTable->entries[index];
}

void setTable(struct nodeType *node){
  switch(node->nodeType){
  case NODE_NESL:
    break;

  case NODE_APPLYBODY2:
    node->child->counts = node->child->nodeType;
    node->child->nodeType = NODE_BODY;
  case NODE_APPLYBODY1:
  case NODE_APPLYBODY3:
  case NODE_APPLYBODY4:
  case NODE_BODY:
  case NODE_FUNC:

  case NODE_LET:{
    //printf("nodetype:%d create scope oh yeah~\n",node->nodeType);
    struct SymTable * newScope = newSymTable(node->table);
    node->table = newScope;
    node->table->parent = node->parent->table;
    break;
  }
  default:
    assert(node->parent);
    if(node->parent->nodeType == NODE_IN){
      if(node->parent ->child == node){
        if(node->parent ->parent ->parent ->nodeType ==NODE_APPLYBODY2){
          assert(node->parent->parent->lsibling->nodeType == NODE_BODY);
          node->table = node->parent ->parent ->lsibling->table;
          break;
        }
      }
    }
   
    node->table = node->parent->table;
    break;
  }
  // recursive
  struct nodeType * child = node->child;
  if(child!=0){
    do{
      setTable(child);
      assert(child->table);
      child = child->rsibling;
    }while(child!=node->child);
  }
  if(node->nodeType == NODE_APPLYBODY2){
    //FIXME?????
    node->child->nodeType =node->child->counts;
    assert(node->child->table!=node->child->rsibling->table );
    assert(node->child->rsibling->child->child->table !=
           node->child->rsibling->child->child->rsibling->table );
  }
}
struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

int renamefcall(char *origin, char *new, struct nodeType *node){
  int counts=0;
  switch(node->nodeType){
  case NODE_FUNC:
    assert(0);
  case NODE_FUNC_CALL:
    // TODO
    assert(node->child->string);
    if(!strcmp(node->child->string, origin)){
      strcpy(node->child->string, new);
    }
    printf("got one\n");
    printTree(node,0);
    printf("###\n");
  break;
  default:
  {
    struct nodeType *child= node->child;
    if(child){
      do{
        counts += renamefcall(origin, new, child);
        child = child->rsibling;
      }while(child!=node->child);
    }
  }
  }
}
int isSameType( struct nodeType *L, struct nodeType *R){
  if(L->valueType != R->valueType) return 0;
  switch(L->valueType){
  case TypeInt:
  case TypeFloat:
  case TypeBool:
  case TypeChar:
  return 1;
  case TypeSEQ:
    assert(L->typeNode->child);
    assert(R->typeNode->child);
    return isSameType(L->typeNode->child, R->typeNode->child);
  case TypeTuple:
    assert(L->typeNode->child->rsibling);
    assert(R->typeNode->child->rsibling);
    return isSameType(L->typeNode->child, R->typeNode->child)&&
            isSameType(L->typeNode->child->rsibling, R->typeNode->child->rsibling);
  default:
  assert(0); // impossible
  }
}

struct nodeType * removePair(struct nodeType* node){
  struct nodeType *child = node->child; 
  struct nodeType *ignore;
  struct nodeType *pattern = node;
  struct nodeType *patright = node->rsibling;
  struct nodeType *patleft = node->lsibling;
  struct nodeType *patchild= node->child;
  struct nodeType *patparent = node->parent;
  int a=0;

  child = node->child;
  if(!child){return node;}
  else
  {
    do{
      a++;
      //child = removePair(child);
      child=child->rsibling;
    }while(child!=node->child);
    printf("a: %d\n",a);

    for(int i=0;i<a;i++){
      child = removePair(child);
      child = child->rsibling;
    }

    if(node->nodeType == NODE_TYPE_PAIR || node->nodeType == NODE_PAIR){
      patchild = node->child;
      assert(patchild==node->child);
      assert(patchild->rsibling==patchild);
      patchild->parent = node->parent;

      /* */
      if(patleft!=node){
        patleft->rsibling = patchild;
        node->lsibling =0;
        patright->lsibling = patchild;
        node->rsibling =0;
        patchild->lsibling->rsibling = patright;
        patchild->lsibling = patleft;
      }
      //patchild->rsibling = patright;
      if(patparent->child ==node)
        patparent ->child = patchild;
      node = patchild;
    }
    return node;
  }
}

void tupleTransform(struct nodeType *node){
  struct nodeType * foo  = node->child->lsibling;
  if(foo->nodeType == NODE_TUPLE){
    struct nodeType *c1 = nthChild(1,foo);
    struct nodeType *c2 = nthChild(2,foo);
    c1->parent = node;
    c2->parent = node;
    c1->rsibling = c2;
    c2->lsibling = c1;
    c2->rsibling = node->child;
    node->child->lsibling = c2;
    c1->lsibling = foo->lsibling;
    foo->lsibling->rsibling = c1;
    tupleTransform(node);
  }
}

void tupleBinding(struct nodeType *LHS, struct nodeType *RHS){
  struct nodeType *lchild;
  struct nodeType *rchild;
  struct nodeType *typeNode;
  struct nodeType *ltype;
  struct nodeType *rtype;

  assert(RHS->valueType == TypeTuple);
  assert(RHS->typeNode);
  assert(RHS->typeNode->valueType ==TypeTuple);

  lchild = LHS->child;
  rchild = LHS->child->rsibling;
  typeNode = RHS->typeNode;
  ltype = typeNode->child;
  rtype = typeNode->child->rsibling;
  if(LHS->nodeType == NODE_TUPLE){
    LHS->valueType = RHS->valueType;
    LHS->typeNode = RHS->typeNode;
  }
  if(LHS->nodeType == NODE_TOKEN){
    assert(0); //note implement;
  }
  // lchild of LHS
  switch(lchild->nodeType){
  case NODE_PAIR:{
    assert(0);    
    break;}
  case NODE_TUPLE:
    assert(lchild);
    assert(rchild);
    lchild->mode= LHS->mode;
    tupleBinding(lchild,ltype);
    lchild->valueType = ltype->valueType;
    lchild->typeNode = ltype->typeNode;
    break;
  case NODE_TOKEN:{
    struct SymTableEntry * entry;
    assert(lchild->string);
    assert(ltype->valueType);
    lchild->mode=LHS->mode;
    lchild->valueType = ltype->valueType;
    if(ltype->valueType>=TypeSEQ)
      assert(ltype->typeNode);
    lchild->typeNode = ltype->typeNode;
    entry = findSymbol(lchild->table, lchild->string, lchild->mode);
    if(entry){
      assert(entry->type== lchild->valueType);
      if(entry->link->typeNode && !(lchild->typeNode))
      lchild->typeNode =  entry->link->typeNode;
    }else{
      if(!lchild->typeNode) lchild->typeNode = lchild;
      addVariable(lchild->string, lchild->valueType, lchild, lchild->mode);
    }
    break;
  }
  default:
    assert(0); //not possible
  break;
  }// end of lchild->nodeType
  
  // rchild of LHS
  switch(rchild->nodeType){
  case NODE_PAIR:
    assert(0); // not implement;
    break;
  case NODE_TUPLE:
    rchild->mode= LHS->mode;
    tupleBinding(rchild, rtype);
    rchild->valueType = rtype->valueType;
    rchild->typeNode = rtype->typeNode;
    break;
  case NODE_TOKEN:{
    struct SymTableEntry * entry;
    assert(rchild->string);
    assert(rtype->valueType);
    rchild->mode = LHS->mode;
    rchild->valueType = rtype->valueType;
    rchild->typeNode = rtype->typeNode;
    entry = findSymbol(rchild->table, rchild->string, rchild->mode);
    if(entry){
      assert(entry->type == rchild->valueType);
    }else{
      if(!rchild->typeNode) rchild->typeNode = rchild;
      addVariable(rchild->string, rchild->valueType, rchild, rchild->mode);
    }
    break;
  }
  default:
    assert(0); //not possible
  break;
  }// end of rchild->nodeType
}// end of tupleBinding

void newtypeBinding(struct nodeType *node){
  assert(node->typeNode);
  switch(node->nodeType){
  case NODE_TOKEN:
    node->valueType = node->typeNode->valueType;
    addVariable(node->string, node->valueType, node, REFERENCE);
  break;
  case NODE_TUPLE:
    node->valueType = node->typeNode->valueType;
    node->child->typeNode = node->typeNode->child;
    node->child->rsibling->typeNode = node->typeNode->child->rsibling;
    newtypeBinding(node->child);
    newtypeBinding(node->child->rsibling);
  break;
  default:
    assert(0); //not implement;
  }
}

void typeAnalysis( struct nodeType *node){
  switch(node->nodeType){
    case NODE_NESL:
    case NODE_LIST:
    case NODE_RBINDS:{

      int count = 0;
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          count++;
          typeAnalysis(child);
          child = child->rsibling; 
        }while(child!=node->child);
      }
      node->counts = count;
      if(node->nodeType == NODE_RBINDS){
        node->valueType = node->child->valueType;
        node->typeNode = node->child;
      }
      break;
    }
    case ELEM_TUPLE:
    case NODE_TUPLE:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      node->valueType = TypeTuple;
      node->typeNode = node;
           break;
    }
      
    case NODE_FUNC:{
      
      struct nodeType *inputParam = node->child;
      struct nodeType *typeDef = node->child->rsibling;
      struct nodeType *returnDef = typeDef->child->rsibling;
      struct nodeType *funcExp = typeDef->rsibling;
      struct FuncTableEntry *fentry;
      
      // functions must have the typeDefinition of Function.
      assert(typeDef->nodeType  == NODE_OP);
      
      if(typeDef->op == OP_RARROW){
        // Bind the inputParameter with TypeDeclaration
        inputParam->isParam=1;
        typeAnalysis(typeDef->child);
        printTree(typeDef->child,0);
        typeDef->child = removePair(typeDef->child);
        printTree(typeDef->child,0);
        printTree(inputParam,0);
        inputParam = removePair(inputParam);
        printTree(inputParam,0);
        inputParam->typeNode=typeDef->child;
        newtypeBinding(inputParam);
    
        // Analyse the returnType of the function, RHS of op_rarrow.
        typeAnalysis(returnDef);
        node->table = node->parent->table;
        node->isParam = 1;
        
        printTree(returnDef,0);
        returnDef = removePair(returnDef);
        printTree(returnDef,0);
        
        node->valueType = returnDef->valueType;
        node->typeNode = returnDef->typeNode;
        if(node->valueType==TypeSEQ) assert(node->typeNode);
        
        // deal with the redefined functions.
        fentry = findFuction(node->string);
        if(fentry){
          int ignore;
          struct nodeType *RRR = node->rsibling; 
          // already declared.
          sprintf(node->string, "%s_%d",node->string,(fentry->renametimes)++);
          ignore = renamefcall(fentry->name, node->string, node->child->rsibling->rsibling);
          
          printf("renamed: %d times\n",ignore);

          while(RRR!=node->parent->child){
            if(RRR->nodeType == NODE_FUNC){
              if(!strcmp(RRR->string,fentry->name)){
                printf("boom~~~\n");
                break;
              }
            }
            else
              ignore = renamefcall(fentry->name, node->string, RRR);
            RRR = RRR->rsibling;
            printf("renamed: %d times\n",ignore);
          }
        } 
        else{
          // first appeared.
          fentry = &funcTable->entries[funcTable->size++];
          fentry->renametimes =1;
          
          strcpy(fentry->name,node->string);
        }
        addVariable(node->string, typeDef->child->rsibling->valueType, node, REFERENCE);  
        
        // Assign the returnType to the functionNode
        typeAnalysis(node->child->rsibling->rsibling);
        node->valueType = node->child->rsibling->rsibling->valueType;
        
        // Add the function node into SymbolTable
      }
      else {
        printf("error in node_func of semanticPass\n");
        assert(0); // no function declaration.
        return;
      } 
     break;
    }
   
    case NODE_TYPE_SEQ:{
      typeAnalysis(node->child);
      node->typeNode = node;
      node->valueType = TypeSEQ;

      break;}

    case NODE_IFELSE:{
      struct nodeType* ifstmt = node->child;
      struct nodeType* thstmt = node->child->rsibling;
      struct nodeType* elstmt = node->child->rsibling->rsibling;
      typeAnalysis(ifstmt);
      typeAnalysis(thstmt);
      typeAnalysis(elstmt);
      assert(ifstmt->valueType == TypeBool); 
      assert(thstmt->valueType == elstmt->valueType);
      node->valueType = elstmt->valueType;
      node->typeNode = elstmt->typeNode;
      
      break;
    }
    case NODE_IFSTMT:
    case NODE_THENSTMT:
    case NODE_ELSESTMT:
    case NODE_TYPE_PAIR:
    case NODE_PAIR:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      node->typeNode = node->child->typeNode;
      break;
    } 
    
    case NODE_EXP:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      node->typeNode = node->child->typeNode;
      break;
    }
    case NODE_LET:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      node->valueType = node->child->rsibling->valueType;
      node->typeNode = node->child->rsibling->typeNode;
      break;
    }
    case NODE_BIND:{
      struct nodeType *child = node->child;
      int counts=0;
      if(child!=0){
        do{
          counts++;
          typeAnalysis(child);
          child = child->rsibling;
        }while(child!=node->child);
      node->childcount = counts;
      }
      break;
    }
    case NODE_SEQ_REF:{
      struct SymTableEntry * entry = findSymbol(node->child->table, node->child->string,REFERENCE);
      struct nodeType *typerefNode;
      struct nodeType *RHS = node->child->rsibling;

      // analyse the index part.
      typeAnalysis(RHS);
      assert(RHS->valueType==TypeInt);
      
      // analyse the sequence.
      assert(entry);      
      assert(entry->type == TypeSEQ);
      assert(entry->link);
      typerefNode = entry->link;
      assert(typerefNode->typeNode);
      typerefNode = typerefNode->typeNode;
      assert(typerefNode->child);
      node->valueType = typerefNode->child->valueType;
      node->typeNode = typerefNode->child;
      if(typerefNode->child->typeNode) node->typeNode = typerefNode->child->typeNode;
      assert(node->typeNode);
      break;
    }

    case NODE_PATTERN:{
      assert(0);
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      break;}
    case NODE_ASSIGN:{
        struct nodeType *LHS= node->child;
        struct nodeType *RHS=LHS->rsibling;
        struct nodeType * pattern = LHS;
        struct nodeType * pchild = pattern ->child;
        
        typeAnalysis(RHS);
        assert(RHS->valueType);
        if(RHS->valueType==TypeTuple){
        assert(RHS->typeNode);
        printTree(RHS,0);
        RHS = removePair(RHS);
        printTree(RHS,0);
        assert(RHS->typeNode);
        }
        
        LHS->valueType = RHS->valueType;
        LHS->typeNode = RHS->typeNode;

        /*Remove Pattern*/
        if(pattern->nodeType == NODE_PATTERN){
          node->mode = NOROOT;
          struct nodeType *patright = pattern->rsibling;
          struct nodeType *patleft = pattern->lsibling;
          struct nodeType *patchild= pattern->child;
          struct nodeType *patparent = pattern->parent;

          do{
            patchild->parent = patparent;
            patchild = patchild->rsibling;
          }while(patchild!=pattern->child);
         
          
          patleft->rsibling = patchild;
          patright->lsibling = patchild;
          patparent ->child = patchild;
          pattern = patchild;
          assert(pattern->lsibling==pattern);
          assert(pattern->rsibling==pattern);
          pattern->rsibling = patright;
          pattern->lsibling = patleft;
        }
        
        /* assign pchild */
        pchild = pattern->child;
        
        /*remove pair inside pchild*/
        printTree(pattern,0);
        pattern = removePair(pattern);
        printTree(pattern,0);
        
        /* top-level need to be declared in global */
        if(node->parent->nodeType == NODE_NESL)
          pattern->isParam = 0;
        
        /* handleing */
        switch(pattern->nodeType){
          case NODE_TOKEN:{
            struct SymTableEntry * entry;
            assert(pattern->string);
            assert(RHS->valueType);
            pattern->valueType=RHS->valueType;
            pattern->typeNode=RHS->typeNode;
            entry = findSymbol(pattern->table, pattern->string,REFERENCE);
            if(!entry){
              addVariable(pattern->string, pattern->valueType, pattern, REFERENCE);
            }
          break;}
          case NODE_TUPLE:
            pattern->mode = node->mode;
            pattern->valueType = RHS->valueType;
            pattern->typeNode = RHS->typeNode;
            tupleBinding(pattern, RHS);
            break;
          default:
            assert(0);
        }
        node->valueType = RHS->valueType;
        node->typeNode=RHS->typeNode;
        break;
      }
    case NODE_OP:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
        typeAnalysis(LHS);
        if(RHS!=LHS)
          typeAnalysis(RHS);
      switch(node->op){
        case OP_ADD:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          assert(node->valueType<=TypeFloat);
          break;
        case OP_SUB:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_MUL:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_DIV:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_RARROW:
          assert(LHS->valueType == RHS->valueType);
          break;
        case OP_AT:
          break;
        case OP_UMINUS:
          node->valueType=LHS->valueType;
          node->typeNode = LHS->typeNode;
          break;
        case OP_SHARP:{
          if(LHS->valueType ==TypeSEQ ){
            node->valueType = TypeInt;
          }else{ 
            assert(0); // semantic error
          }
          break;
        }
        case OP_GT:
        case OP_EQ:
        case OP_GE:
        case OP_LE:
        case OP_NE:
        case OP_LT:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = TypeBool;
          break;
        case OP_NOT:
          node->valueType = LHS->valueType;
          break;
        case OP_COMMA:
          break;
        case OP_OR:
        case OP_NOR:
        case OP_XOR:
        case OP_AND:
        case OP_NAND:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_LARROW:
          assert(0);
          break;
        case OP_UPT:
          assert(RHS->valueType == TypeInt);
          node->valueType = LHS->valueType;
          break;
        case OP_PP:{
          struct nodeType *LHSchild, *RHSchild;
          int done = 0;
          assert(LHS->valueType == TypeSEQ);
          assert(RHS->valueType == TypeSEQ);
          LHSchild = LHS->typeNode->child;
          RHSchild = RHS->typeNode->child;
          assert(isSameType(LHSchild,RHSchild));
          node->valueType = LHS->valueType;
          node->typeNode = LHS->typeNode;
          break;
        }
        default:
        assert(0); // not implement;
      }// end of node->op
      break;
    } // end of NODE_OP
     
    case NODE_FUNC_CALL:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = LHS->rsibling;
      // first, check the input parameter.
      typeAnalysis(RHS);
      RHS = removePair(RHS);
      
      // second, check the function return type.
      assert(node->child->nodeType == NODE_TOKEN);
      assert(node->child->string);
      
      if(strcmp(node->child->string, "isContiguousList")==0){
          node->valueType = TypeInt;
          return;
          }
      else if(strcmp(node->child->string, "verifyQHull")==0){
          struct nodeType *param1 = RHS->child;
          struct nodeType *param2 = param1->rsibling;
          node->valueType= TypeBool;
          assert(param1->valueType == TypeInt); 
          assert(param2->valueType == TypeSEQ); 
          return;
        }
      else if(strcmp(node->child->string, "dist")==0){
        struct nodeType *typer = newNode(NODE_TYPE_SEQ);
        typer->child = RHS->child;
        if(RHS->child->typeNode) typer->child = RHS->child->typeNode;
        typer->valueType = TypeSEQ;
        node->valueType = TypeSEQ;
        node->typeNode = typer;
        return;
      }
      else if(strcmp(node->child->string, "time")==0){
        // sequential version
        node->valueType = TypeTuple;
        struct nodeType *refNode = newNode(NODE_TYPE_TUPLE);
        struct nodeType *Lchild = newNode(NODE_TOKEN);
        struct nodeType *Rchild = newNode(NODE_TOKEN);
        addChild(refNode, Lchild);
        addChild(refNode, Rchild);
        Rchild->tokenType = TOKE_FLOAT;
        Rchild->valueType = TypeFloat;
        Rchild->table = node->table;
        refNode->table = node->table;
        refNode->valueType = TypeTuple;
        refNode->typeNode = refNode;
        assert(node->child->rsibling->valueType);
        Lchild->table = node->table;
        Lchild->tokenType = -1;
        Lchild->valueType = node->child->rsibling->valueType;
        Lchild->typeNode = node->child->rsibling->typeNode;
        node->typeNode = refNode;
        typeAnalysis(refNode);
        node->valueType = refNode->valueType;
        return;
      }else if(strcmp(node->child->string, "rand")==0){
        node->valueType = RHS->valueType;
        return;
      }else if(strcmp(node->child->string, "plusp") ==0){
        node->valueType = TypeBool;
        assert(RHS->valueType>=TypeInt && RHS->valueType<=TypeFloat);
        return;
      }else if(strcmp(node->child->string, "flatten") == 0){
        assert(RHS->valueType==TypeSEQ);
        node->valueType = RHS->typeNode->child->valueType;
        //node->typeNode = RHS->typeNode->child;
        node->typeNode = RHS->typeNode;
        if(RHS->typeNode->typeNode) node->typeNode = RHS->typeNode->typeNode;
        return;
      }else if(strcmp(LHS->string, "max_index") == 0){
        node->valueType = TypeInt;
        //RHS=removePair(RHS);
        assert(RHS->valueType == TypeSEQ);
        return;
      }else if(strcmp(LHS->string, "min_index") == 0){
        node->valueType = TypeInt;
        //RHS=removePair(RHS);
        assert(RHS->valueType == TypeSEQ);
        return;
      }else if(strcmp(LHS->string, "float") == 0){
        node->valueType = TypeFloat;
        return;
      }else if(strcmp(LHS->string, "round") == 0){
        node->valueType = TypeInt;
        assert(RHS->valueType == TypeFloat);
        return;
      }else if(strcmp(LHS->string, "print_string") == 0){
        node->valueType = TypeBool;
        //assert(RHS->valueType == TypeSEQ_C);
        return;
      }else if(strcmp(LHS->string, "genShuffledList") == 0){
        struct nodeType *refNode1 = newNode(NODE_TYPE_SEQ);
        struct nodeType *refNode2 = newNode(NODE_TYPE_INT);
        addChild(refNode1,refNode2);
        refNode1->valueType = TypeSEQ;
        refNode2->valueType = TypeInt;
        node->valueType = TypeSEQ;
        node->typeNode = refNode1;
        assert(RHS->valueType == TypeTuple);
        return;
      }else if(strcmp(LHS->string, "genReverseList") == 0){
        struct nodeType *refNode1 = newNode(NODE_TYPE_SEQ);
        struct nodeType *refNode2 = newNode(NODE_TYPE_INT);
        addChild(refNode1,refNode2);
        refNode1->valueType = TypeSEQ;
        refNode2->valueType = TypeInt;
        node->valueType = TypeSEQ;
        node->typeNode = refNode1;
        assert(RHS->valueType == TypeInt);
        return;
      }else if(strcmp(LHS->string, "sum") == 0){
        switch(RHS->typeNode->child->valueType){
          case TypeInt:
            node->valueType = TypeInt;
            break;
          case TypeFloat:
            node->valueType = TypeFloat;
            break;
          default:
            assert(0); //not implement;
          break;
        }
          
        return;
      }else if(strcmp(LHS->string, "mod") == 0){
        
        if(RHS->valueType!=TypeTuple) {
          printf("***semantic check error***\n");
          abort();
        }
        node->valueType = TypeInt;
        LHS->valueType = TypeInt;
        return;
      }else{
          // TODO other built-in functions
      typeAnalysis(node->child);
      struct SymTableEntry *entry = findSymbol(node->table, node->child->string, REFERENCE); 
      assert(entry);
      assert(entry->type);
      node->valueType = entry->type;
      node->typeNode = entry->link->typeNode; 
      }
      // 1. search the node->child->string in built-in list
      //    if found then use the signature to check and 
      //    assign the type informations.
      // 2. if not found, then search the symbol table
      //    it should be a user define function.
      
      //node->valueType = node->child->valueType;
      break;
    }// end of NODE_FUNC_CALL

    case NODE_APPLYBODY1:{
      typeAnalysis(node->child);
      node->valueType = TypeSEQ;
      break;
    }
    case NODE_APPLYBODY2:{
      typeAnalysis(node->child->rsibling);
      typeAnalysis(node->child);
      node->valueType = TypeSEQ;
      node->typeNode = node;
      break;
    }
    case NODE_APPLYBODY3:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      assert(node->child->rsibling->valueType == TypeBool);
      assert(node->child->valueType);
      node->valueType = TypeSEQ;
      assert(node->child->child->child->rsibling->valueType==TypeSEQ);
      assert(node->child->child->child->rsibling->typeNode);
      node->typeNode = node->child->child->child->rsibling->typeNode;
      break;
    }
    case NODE_APPLYBODY4:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      typeAnalysis(node->child->lsibling);
      node->valueType = TypeSEQ;
      assert(node->child->lsibling->valueType == TypeBool);
      break;
    }
    case NODE_IN:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      
      // TODO e in a; or (x,y) in points; ... ; 
      // so we need to removePair(LHS); 
      // then bind the child of the RHS's typeNode with LHS.
      
      typeAnalysis(RHS);
      assert(RHS->valueType==TypeSEQ);
      assert(RHS->typeNode);
      assert(RHS->typeNode->valueType);
      node->valueType = RHS->valueType;
      node->typeNode = RHS->typeNode;
      LHS=removePair(LHS);
      
      // FIXME make tupleBinding can handle this
      if(LHS->nodeType ==NODE_TUPLE){
      tupleBinding(LHS,RHS->typeNode->child);
      LHS->valueType = RHS->typeNode->child->valueType;
      LHS->typeNode = RHS->typeNode->child;
      if(RHS->typeNode->child->typeNode)
        LHS->typeNode = RHS->typeNode->child->typeNode;
      LHS->nodeType = RB_TUPLE;
      typeAnalysis(LHS);
      }else{
        LHS->valueType = RHS->typeNode->child->valueType;
        LHS->typeNode = RHS->typeNode->child;
        if(RHS->typeNode->child->typeNode)
          LHS->typeNode = RHS->typeNode->child->typeNode;
        if(LHS->nodeType == NODE_TOKEN){
          addVariable(LHS->string, LHS->valueType, LHS, REFERENCE); 
        }
      }
      assert(LHS->valueType); 
      assert(LHS->typeNode);
      break;
    }// end of NODE_IN

    case RB_TUPLE:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      struct nodeType* lref = node->typeNode->child;
      struct nodeType* rref = lref->rsibling;

      if(LHS->nodeType == NODE_TUPLE){
        LHS->nodeType = RB_TUPLE;
        typeAnalysis(LHS);
      }else if(LHS->nodeType == NODE_TOKEN){
        if(!findSymbol(LHS->table, LHS->string, REFERENCE)){
          addVariable(LHS->string, lref->valueType, LHS,FORCEDECLARE);
          LHS->valueType = lref->valueType;
          LHS->typeNode = lref;
        }
      }else{
        assert(0);
      }

      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = RB_TUPLE;
        typeAnalysis(RHS);
      }else if(RHS->nodeType == NODE_TOKEN){
        if(!findSymbol(RHS->table, RHS->string, REFERENCE)){
          addVariable(RHS->string, rref->valueType, RHS,FORCEDECLARE);
          RHS->valueType = rref->valueType;
          RHS->typeNode = rref;
        }
      }else{
        assert(0);
      }

      break;
    }// end of RB_TUPLE

    case NODE_TOKEN: {
        switch(node->tokenType){
          case TOKE_ID:{
            struct SymTableEntry* entry = findSymbol(node->table, node->string, REFERENCE);
            if(entry){
              node->valueType = entry->type; 
              node->typeNode = entry->link->typeNode;
            
              if(node->valueType == TypeSEQ){
                node->typeNode = entry->link->typeNode;
              }
              //addVariable(node->string, TypeInt, node);
            }else{
              printf("###[Warning] maybe not declare variable: %s ###\n",node->string);
            }
            break;
            }
          case TOKE_INT:
            node->valueType = TypeInt;
            break;
          case TOKE_FLOAT:
            node->valueType = TypeFloat;
            break;
          case TOKE_BOOL:
            node->valueType = TypeBool;
            break;
          case TOKE_CHAR:
            node->valueType = TypeChar;
            break;
          case TOKE_STRING:{
            struct nodeType* refNode = newNode(NODE_CHAR);
            refNode->valueType = TypeChar;
            refNode->typeNode = refNode;
            node->valueType = TypeSEQ;
            node->typeNode = refNode;
            break;}
          default:
            break;
        }// end of switch tokenType
      break;
    }// end of NODE_TOKEN
    
    case NODE_INT:{
      node->valueType = TypeInt;
      break;
    }
    case NODE_BOOL:{
      node->valueType = TypeBool;
      if(strcmp(node->string,"f")==0){
        node->iValue=0;
      }else if(strcmp(node->string,"F")==0){
        node->iValue=0;
      }else if(strcmp(node->string,"t")==0){
        node->iValue=1;
      }else if(strcmp(node->string,"T")==0){
        node->iValue=1;
      }

      break;
    }
    case NODE_FLOAT:{
      node->valueType = TypeFloat;
      node->typeNode = node;
      break;
    }
    case NODE_CHAR:{
      node->valueType = TypeChar;
      node->typeNode = node;
      break;
    }
    case NODE_STRING:{
      struct nodeType* refNode = newNode(NODE_CHAR);
      refNode->valueType = TypeChar;
      refNode->typeNode = refNode;
      node->valueType = TypeSEQ;
      node->typeNode = refNode;
      break;}
    
    case NODE_FILTER:{
      int count = 0;
      struct nodeType *child = node->child;
      if(child){
        do{
          count++;
        }while(child!= node->child);
      }
      node->counts = count;
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      node->typeNode = node;
      break;
    }
    case NODE_NEW_SEQ:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;
      int count=0;
      assert(LHS);
      //paramcount = idx:  0, 1, 2
      //                  [1, 2, 3]
      //                   new_seq 
      //                    /  \
      //                   1  tuple
      //                       / \
      //                      2   3
      
      // transform pair-tuple into elem_tuple(remove pair)
      // this will help type inference.
      if(LHS->nodeType == NODE_PAIR && LHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = LHS; //child = node->child;
        struct nodeType * rhs = LHS->rsibling;
        struct nodeType * lhs = LHS->lsibling;
        struct nodeType * pchild = LHS->child;
        LHS->child->nodeType = ELEM_TUPLE; 
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        node->child = child;
        LHS=node->child;
      }

      // below consider no pair_node in tree 
      typeAnalysis(LHS);
      LHS->paramcount = 0;
      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = NODE_SEQ_TUPLE;
        RHS->paramcount = 1;
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->valueType);
        node->counts = 1+RHS->counts;
      }
      else{
        if(RHS->nodeType == NODE_PAIR && RHS->child->nodeType == NODE_TUPLE){
          // remove pair;
          struct nodeType * child = RHS; //child = node->child;
          struct nodeType * rhs = RHS->rsibling;
          struct nodeType * lhs = RHS->lsibling;
          struct nodeType * pchild = RHS->child;
          RHS->child->nodeType = ELEM_TUPLE;
          while(child->nodeType == NODE_PAIR){
            pchild->parent = node;
            rhs->lsibling = pchild;
            lhs->rsibling = pchild;
            child = pchild;
            child->rsibling = rhs;
            child->lsibling = lhs;
            rhs = child->rsibling;
            lhs = child->lsibling;
            pchild = child->child;
          }
          RHS=LHS->rsibling;
        }
        typeAnalysis(RHS);
        RHS->paramcount =1;
      }// end of else.
      
      switch(LHS->valueType){
        case TypeInt: 
        case TypeFloat:
        case TypeBool:
        case TypeChar:
        default:
          node->valueType = TypeSEQ;
          node->typeNode = node;
          break;
      }
      break;
    }

    case NODE_SEQ_TUPLE:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      
      if(LHS->nodeType == NODE_PAIR && LHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = LHS; //child = node->child;
        struct nodeType * rhs = LHS->rsibling;
        struct nodeType * lhs = LHS->lsibling;
        struct nodeType * pchild = LHS->child;
        
        LHS->child->nodeType = ELEM_TUPLE;
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        node->child = child; 
        LHS = node->child;
      }
      if(RHS->nodeType == NODE_PAIR && RHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = RHS; //child = node->child;
        struct nodeType * rhs = RHS->rsibling;
        struct nodeType * lhs = RHS->lsibling;
        struct nodeType * pchild = RHS->child;
        RHS->child->nodeType = ELEM_TUPLE;
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        RHS = LHS->rsibling;
      }
      
      typeAnalysis(LHS);
      node->valueType = LHS->valueType;
      LHS->paramcount = node->paramcount;
      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = NODE_SEQ_TUPLE;
        RHS->paramcount = node->paramcount + 1;
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->child->valueType);
        node->counts = RHS->counts+1;
      }else{
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->valueType);
        node->counts = 2;
        RHS->paramcount = node->paramcount +1;
      }
      node->typeNode=node;
      break;
    } 
    case NODE_SEQ:{
        struct nodeType * child= node->child;
        assert(child);
        typeAnalysis(child);
        switch(child->valueType){
          case TypeInt:
          case TypeFloat:
          case TypeBool:
          case TypeChar:
          default:
            node->valueType = TypeSEQ;
            node->typeNode = node;
            break;
        }

      break;
    }
    
  }// end of switch nodeType

}//end of typeAnalysis

//void setTable(struct nodeType *node, struct SymTable *table){


void semanticPass( struct nodeType *node){
  // create and set a table in rootNode. 
  // then start the Analysis
  // which consist of table insertion, 
  // typeBinding and typeChechking.

  /*initialize the function table*/
  typeTable = (struct TypeTable*)malloc(sizeof(struct TypeTable));
  typeTable->size = 0;
  
  funcTable = (struct FuncTable*)malloc(sizeof(struct FuncTable));
  funcTable->size = 0;
  for(int i=0;i<100;i++){
    funcTable->entries[i].renametimes =0;
    strcpy(funcTable->entries[i].name, "");
  }

  rootTable = newSymTable(NULL);
  node->table = rootTable;
  setTable(node);
  typeAnalysis(node);
}
